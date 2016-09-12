/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myfile.h"

/* If you declare any globals in php_myfile.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(myfile)
*/

/* True global resources - no need for thread safety here */
static int le_myfile;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("myfile.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_myfile_globals, myfile_globals)
    STD_PHP_INI_ENTRY("myfile.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_myfile_globals, myfile_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_myfile_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_myfile_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "myfile", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/
PHP_FUNCTION(file_open)
{
	char *filename = NULL;
	char *mode = NULL;
	int argc = ZEND_NUM_ARGS();
	int filename_len;
	int mode_len;
	FILE *fp;

	if (zend_parse_parameters(argc TSRMLS_CC, "ss", &filename,&filename_len, &mode, &mode_len) == FAILURE) {
		return;
	}
	fp = VCWD_FOPEN(filename, mode);
	if (fp == NULL) {
		RETURN_FALSE;
	}

	ZEND_REGISTER_RESOURCE(return_value, fp, le_myfile);
	FILE *file = fopen("/tmp/myfile", "a");
	fprintf(file, "file_open le_myfile:%d\n", le_myfile);
	fclose(file);
}
PHP_FUNCTION(file_eof){
	int argc = ZEND_NUM_ARGS();
	zval *filehandle = NULL;
	FILE *fp;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &filehandle) ==FAILURE) {
		return;
	}
						 
	ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, -1, "standard-c-file",le_myfile);
							  
	if (fp == NULL){
		RETURN_FALSE;
	} 

	if (feof(fp) != 0) {
		/* Return eof also if there was an error */
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

PHP_FUNCTION(file_close){
	int argc = ZEND_NUM_ARGS();
	zval *filehandle = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &filehandle) == FAILURE) {
		return;
	}
				    
	if (zend_list_delete(Z_RESVAL_P(filehandle)) == FAILURE) {
		RETURN_FALSE;
	}
						 
	RETURN_TRUE;
}

PHP_FUNCTION(file_read){
     int argc = ZEND_NUM_ARGS();
     long size;
     zval *filehandle = NULL;
     FILE *fp;
     char *result;
     size_t bytes_read;
 
     if (zend_parse_parameters(argc TSRMLS_CC, "rl", &filehandle,&size) == FAILURE) {
          return;
     }
 
     ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, -1, "standard-cfile", le_myfile);
 
     result = (char *) emalloc(size+1);
 
     bytes_read = fread(result, 1, size, fp);
 
     result[bytes_read] = '\0';
 
     RETURN_STRING(result, 0);
}
 
PHP_FUNCTION(file_write){
     char *buffer = NULL;
     int argc = ZEND_NUM_ARGS();
     int buffer_len;
     zval *filehandle = NULL;
     FILE *fp;
 
     if (zend_parse_parameters(argc TSRMLS_CC, "rs", &filehandle,&buffer, &buffer_len) == FAILURE) {
          return;
     }
 
     ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, -1, "standard-cfile", le_myfile);
 
     if (fwrite(buffer, 1, buffer_len, fp) != buffer_len) {
          RETURN_FALSE;
     }
 
     RETURN_TRUE;
}


/* {{{ php_myfile_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_myfile_init_globals(zend_myfile_globals *myfile_globals)
{
	myfile_globals->global_value = 0;
	myfile_globals->global_string = NULL;
}
*/
/* }}} */

static void myfile_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	FILE *fp = (FILE *) rsrc->ptr;
	fclose(fp);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(myfile)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	le_myfile = zend_register_list_destructors_ex(myfile_dtor, NULL, "standard-c-file", module_number);
	FILE *file = fopen("/tmp/myfile", "a");
	fprintf(file, "PHP_MINIT_FUNCTION le_myfile:%d\n", le_myfile);
	fclose(file);
	return SUCCESS;
}
/* }}} */



/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(myfile)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	FILE *file = fopen("/tmp/myfile", "a");
	fprintf(file, "PHP_MSHUTDOWN_FUNCTION le_myfile:%d\n", le_myfile);
	fclose(file);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(myfile)
{
	FILE *file = fopen("/tmp/myfile", "a");
	fprintf(file, "PHP_RINIT_FUNCTION le_myfile:%d\n", le_myfile);
	fclose(file);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(myfile)
{
	FILE *file = fopen("/tmp/myfile", "a");
	fprintf(file, "PHP_RSHUTDOWN_FUNCTION le_myfile:%d\n", le_myfile);
	fclose(file);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(myfile)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "myfile support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ myfile_functions[]
 *
 * Every user visible function must have an entry in myfile_functions[].
 */
const zend_function_entry myfile_functions[] = {
	PHP_FE(confirm_myfile_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(file_open,	NULL)
	PHP_FE(file_read,	NULL)
	PHP_FE(file_write,	NULL)
	PHP_FE(file_eof,	NULL)
	PHP_FE(file_close,	NULL)
	PHP_FE_END	/* Must be the last line in myfile_functions[] */
};
/* }}} */

/* {{{ myfile_module_entry
 */
zend_module_entry myfile_module_entry = {
	STANDARD_MODULE_HEADER,
	"myfile",
	myfile_functions,
	PHP_MINIT(myfile),
	PHP_MSHUTDOWN(myfile),
	PHP_RINIT(myfile),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(myfile),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(myfile),
	PHP_MYFILE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYFILE
ZEND_GET_MODULE(myfile)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
