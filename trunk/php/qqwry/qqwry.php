<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('qqwry')) {
	dl('qqwry.' . PHP_SHLIB_SUFFIX);
}
$module = 'qqwry';
$functions = get_extension_funcs($module);
$function =  $module ;
list($s1,$s2)=qqwry('62.10.12.131','/Users/surfchen/projects/qqwry/nali/php/QQWry.Dat');
echo $s1,$s2;
var_dump($s2);
exit;
if (extension_loaded($module)) {
echo "\n";
	var_dump($function('183.213.101.3','/Users/surfchen/projects/qqwry/nali/php/QQWry.Dat'));
exit;
	var_dump($function('138.89.37.58','/Users/surfchen/projects/qqwry/nali/php/QQWry.Dat'));
	var_dump($function('143.236.149.131','/Users/surfchen/projects/qqwry/nali/php/QQWry.Dat'));
	var_dump($function('67.104.234.151','/Users/surfchen/projects/qqwry/nali/php/QQWry.Dat'));
echo "\n";
	var_dump($function('222.216.47.4','/Users/surfchen/projects/qqwry/nali/php/QQWry.Dat'));
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "\n";
?>
