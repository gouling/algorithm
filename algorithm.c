#include "php_algorithm.h"

/*
 * 等额本息
 * 每月月供额=贷款本金×月利率×(1+月利率)^还款月数÷[(1+月利率)^还款月数-1]
 * 每月应还利息=贷款本金×月利率×[(1+月利率)^还款月数-(1+月利率)^(还款月序号-1)]÷[(1+月利率)^还款月数-1]
 * 每月应还本金=贷款本金×月利率×(1+月利率)^(还款月序号-1)÷[(1+月利率)^还款月数-1]
 * 总利息=还款月数×每月月供额-贷款本金
 */
PHP_FUNCTION(getAverageCapitalPlusInterest) {
    zval *list, *set;  //还款详情
    
    Calendar calendar;  //计息开始日期
    double cash;    //借款金额
    double apr; //借款年利率
    long period; //借款期限
    
    double total; //每月还款总金额
    
    //输入原始日期
    char *args,date[10]={0};
    int args_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sddl",
        &args,&args_len,&cash,&apr,&period)==FAILURE) {
        php_error_docref(NULL TSRMLS_CC,E_ERROR,"Need the parameter matching interest algorithm");
    }
    strcat(date,args);
    sscanf(date,"%4d-%02d-%02d",&calendar.year,&calendar.month,&calendar.day);
    
	apr=apr/12; //年利率->月利率
	total=cash*apr*pow(1+apr,period)/(pow(1+apr,period)-1); //计算每期还款金额(本金+利息)

	MAKE_STD_ZVAL(list);
	array_init(list);

	//计算每期还款时间、本金、利息
	double principal=0;
	for(int index=0,day=0,month=calendar.month;index<period;index++) {
	    MAKE_STD_ZVAL(set);
	    array_init(set);
	 
	    principal=cash*apr*pow(1+apr,index)/(pow(1+apr,period)-1);  //本期还款本金
	    
	    //本期还款日期即下期计息日期
	    add_assoc_string(set,"date",date,1);    //本期计息日期
	    day=getYearMonthTotalDay(calendar.year,month++);    //本期计息天数
	    
	    calendar=getDateAdd(calendar,day);
	    sprintf(date,"%4d-%02d-%02d",calendar.year,calendar.month,calendar.day);   
	     
	    add_assoc_string(set,"yet",date,1);  //本期还款日期
	    add_assoc_long(set,"day",day);  //本期计息天数
	    
	    add_assoc_double(set,"cash",principal); //本期还款本金
	    add_assoc_double(set,"apr",total-principal);    //本期还款利息=每期还款金额-本期还款金额
	    
	    add_index_zval(list, index+1, set);
	}

    RETURN_ZVAL(list,1,0);
}

/*
 * 等额本金
 * 每月月供额=(贷款本金÷还款月数)+(贷款本金-已归还本金累计额)×月利率
 * 每月应还本金=贷款本金÷还款月数
 * 每月应还利息=剩余本金×月利率=(贷款本金-已归还本金累计额)×月利率
 * 每月月供递减额=每月应还本金×月利率=贷款本金÷还款月数×月利率
 * 总利息=还款月数×(总贷款额×月利率-月利率×(总贷款额÷还款月数)*(还款月数-1)÷2+总贷款额÷还款月数)
 */
PHP_FUNCTION(getAverageCapital) {
    zval *list, *set;  //还款详情
    
    Calendar calendar;  //计息开始日期
    double cash;    //借款金额
    double apr; //借款年利率
    long period; //借款期限
    
    double total; //每月还款总金额
    
    //输入原始日期
    char *args,date[10]={0};
    int args_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sddl",
        &args,&args_len,&cash,&apr,&period)==FAILURE) {
        php_error_docref(NULL TSRMLS_CC,E_ERROR,"Need the parameter matching interest algorithm");
    }
    strcat(date,args);
    sscanf(date,"%4d-%02d-%02d",&calendar.year,&calendar.month,&calendar.day);
    
	apr=apr/12; //年利率->月利率
	
	MAKE_STD_ZVAL(list);
	array_init(list);

	//计算每期还款时间、本金、利息
	double principal=cash/period;   //每期还款本金
	double interest=0;  //每期还款利息
	for(int index=0,day=0,month=calendar.month;index<period;index++) {
	    MAKE_STD_ZVAL(set);
	    array_init(set);
	    
	    //本期还款日期即下期计息日期
	    add_assoc_string(set,"date",date,1);    //本期计息日期
	    day=getYearMonthTotalDay(calendar.year,month++);    //本期计息天数
	    
	    calendar=getDateAdd(calendar,day);
	    sprintf(date,"%4d-%02d-%02d",calendar.year,calendar.month,calendar.day);   
	     
	    add_assoc_string(set,"yet",date,1);  //本期还款日期
	    add_assoc_long(set,"day",day);  //本期计息天数
	    
	    interest=cash*apr;
	    cash-=principal;
	    
	    add_assoc_double(set,"cash",principal); //本期还款本金
	    add_assoc_double(set,"apr",interest);    //本期还款利息=每期还款金额-本期还款金额
	    
	    add_index_zval(list, index+1, set);
	}

    RETURN_ZVAL(list,1,0);
}

PHP_MINFO_FUNCTION(algorithm) {
    php_info_print_table_start();
    php_info_print_table_header(2,"Version",PHP_ALGORITHM_EXTVER);
    php_info_print_table_row(2,"getAverageCapitalPlusInterest","enabled");
    php_info_print_table_row(2,"getAverageCapital","enabled");
    php_info_print_table_end();
}

const zend_function_entry algorithm_functions[] = {
    PHP_FE(getAverageCapitalPlusInterest,NULL)
    PHP_FE(getAverageCapital,NULL)
    PHP_FE_END
};

zend_module_entry algorithm_module_entry = {
    #if ZEND_MODULE_API_NO >= 20010901
        STANDARD_MODULE_HEADER,
    #endif
        PHP_ALGORITHM_EXTNAME,
        algorithm_functions,
        NULL, /* MINIT */
        NULL, /* MSHUTDOWN */
        NULL, /* RINIT */
        NULL, /* RSHUTDOWN */
        PHP_MINFO(algorithm),
    #if ZEND_MODULE_API_NO >= 20010901
        PHP_ALGORITHM_EXTVER,
    #endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ALGORITHM
    ZEND_GET_MODULE(algorithm)
#endif
