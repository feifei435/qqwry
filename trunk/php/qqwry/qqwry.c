/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:Surf Chen <surfchen@gmail.com>                                |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_qqwry.h"
#include "libqqwry.h"

#define QQWRY_ADDR_LEN 128 
/* If you declare any globals in php_qqwry.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(qqwry)
*/

/* True global resources - no need for thread safety here */
static int le_qqwry;

/* {{{ qqwry_functions[]
 *
 * Every user visible function must have an entry in qqwry_functions[].
 */
zend_function_entry qqwry_functions[] = {
	PHP_FE(qqwry,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in qqwry_functions[] */
};
/* }}} */

/* {{{ qqwry_module_entry
 */
zend_module_entry qqwry_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"qqwry",
	qqwry_functions,
	PHP_MINIT(qqwry),
	PHP_MSHUTDOWN(qqwry),
	PHP_RINIT(qqwry),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(qqwry),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(qqwry),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_QQWRY
ZEND_GET_MODULE(qqwry)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("qqwry.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_qqwry_globals, qqwry_globals)
    STD_PHP_INI_ENTRY("qqwry.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_qqwry_globals, qqwry_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_qqwry_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_qqwry_init_globals(zend_qqwry_globals *qqwry_globals)
{
	qqwry_globals->global_value = 0;
	qqwry_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(qqwry)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(qqwry)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(qqwry)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(qqwry)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(qqwry)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "qqwry support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string qqwry(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(qqwry)
{
	char *ip_string = NULL;
	char *qqwry_path = NULL;
	int ipstring_len, qqwry_len;
	char *strg;
	int len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &ip_string, &ipstring_len,&qqwry_path,&qqwry_len) == FAILURE) {
		return;
	}
	zval *zaddr1,*zaddr2;
	char *addr1=(char *)emalloc(QQWRY_ADDR_LEN);
	char *addr2=(char *)emalloc(QQWRY_ADDR_LEN);
	memset(addr1,0,QQWRY_ADDR_LEN);
	memset(addr2,0,QQWRY_ADDR_LEN);

	qqwry_get_location(addr1,addr2,ip_string,qqwry_path);
	//len = spprintf(&strg, 0, "%s %s", addr1,addr2);
	MAKE_STD_ZVAL(zaddr1);
	ZVAL_STRING(zaddr1,addr1,0);
	MAKE_STD_ZVAL(zaddr2);
	ZVAL_STRING(zaddr2,addr2,0);

	array_init(return_value);
    add_next_index_zval(return_value,zaddr1);
    add_next_index_zval(return_value,zaddr2);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
