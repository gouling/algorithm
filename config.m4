PHP_ARG_ENABLE(algorithm,
 [Whether to enable the algorithm extension],
 [  --enable-algorithm      Enable algorithm extension support])
if test $PHP_ALGORITHM != "no"; then
 PHP_SUBST(ALGORITHM_SHARED_LIBADD)
 PHP_NEW_EXTENSION(algorithm, algorithm.c, $ext_shared)
fi