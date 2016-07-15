typedef struct {
	int year;
	int month;
	int day;
}Calendar;

/*
 * 验证是否是润年
 *
 * 普通年能被4整除且不能被100整除的为闰年
 * 世纪年能被400整除的是闰年
 */
int isLeapYear(int year) {
	return (year%4==0 && year%100!=0) || year%400==0;
}

/*
 * 年月取当月天数
 *
 * 1、3、5、7、8、10、12月=31天
 * 4、6、9、11月=30天
 * 润年2月=29天，非润年=28天
 */
int getYearMonthTotalDay(int year, int month) {
	int data[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if(isLeapYear(year)==1) {	//润年，重写2月天数
		data[1]=29;
	}

	return data[month-1];	//月天数=数组从0起，月份-1
}

/*
 * 日期加天数取日期
 */
Calendar getDateAdd(Calendar date, int day) {
	while(day-->0) {
		date.day++;
		if(date.day>getYearMonthTotalDay(date.year,date.month)) {
			date.month++;
			date.day=1;
		}

		if(date.month>12) {
			date.year++;
			date.month=1;
		}
	}

	return date;
}

/*
 * 日期差取天数
 */
int getDateDiff(Calendar set, Calendar get) {
	int day=0;
	Calendar *date;

	if(set.year>get.year) {
		date=&get;
	}else if(set.year==get.year && set.month>get.month) {
		date=&get;
	}else if(set.year==get.year && set.month==get.month && set.day>get.day) {
		date=&get;
	}else{
		date=&set;
	}

	while(!(set.year==get.year && set.month==get.month && set.day==get.day)) {
		*date=getDateAdd(*date,1);
		day++;
	}

	return date==&get?-day:day;
}