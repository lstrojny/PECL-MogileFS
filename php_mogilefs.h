/**
 * Copyright (c) 2008 Lars Strojny <lstrojny@php.net>
 * Copyright (c) 2008 Timu Eren <selamtux@gmail.com>
 * Copyright (c) 2007 Khalid Ahsein <mogilefs@capoune.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the PHP MogileFS authors nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PHP_MOGILEFS_H
#define PHP_MOGILEFS_H
#define PHP_MOGILEFS_VERSION "0.9.1"

extern zend_module_entry mogilefs_module_entry;
#define phpext_mogilefs_ptr &mogilefs_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(mogilefs);
PHP_MSHUTDOWN_FUNCTION(mogilefs);
PHP_MINFO_FUNCTION(mogilefs);

PHP_METHOD(MogileFs, __construct);
PHP_METHOD(MogileFs, isConnected);
PHP_METHOD(MogileFs, connect);
PHP_METHOD(MogileFs, get);
PHP_METHOD(MogileFs, getPaths);
PHP_METHOD(MogileFs, getDomains);
PHP_METHOD(MogileFs, fileInfo);
PHP_METHOD(MogileFs, listKeys);
PHP_METHOD(MogileFs, listFids);
PHP_METHOD(MogileFs, getHosts);
PHP_METHOD(MogileFs, getDevices);
PHP_METHOD(MogileFs, sleep);
PHP_METHOD(MogileFs, stats);
PHP_METHOD(MogileFs, replicate);
PHP_METHOD(MogileFs, createDevice);
PHP_METHOD(MogileFs, createDomain);
PHP_METHOD(MogileFs, deleteDomain);
PHP_METHOD(MogileFs, createClass);
PHP_METHOD(MogileFs, updateClass);
PHP_METHOD(MogileFs, deleteClass);
PHP_METHOD(MogileFs, createHost);
PHP_METHOD(MogileFs, updateHost);
PHP_METHOD(MogileFs, deleteHost);
PHP_METHOD(MogileFs, setWeight);
PHP_METHOD(MogileFs, setState);
PHP_METHOD(MogileFs, checker);
PHP_METHOD(MogileFs, monitorRound);
PHP_METHOD(MogileFs, put);
PHP_METHOD(MogileFs, close);
PHP_METHOD(MogileFs, disconnect);
PHP_METHOD(MogileFs, delete);
PHP_METHOD(MogileFs, rename);
PHP_METHOD(MogileFs, setReadTimeout);
PHP_METHOD(MogileFs, getReadTimeout);
PHP_METHOD(MogileFs, isInDebuggingMode);

#define mogilefs_sock_name "MogileFS Socket Buffer"

#define MOGILEFS_SOCK_WRITE_FREE(socket, cmd, cmd_len) \
	mogilefs_sock_write (socket, cmd, cmd_len, 1 TSRMLS_CC)
#define MOGILEFS_SOCK_WRITE(socket, cmd, cmd_len) \
	mogilefs_sock_write (socket, cmd, cmd_len, 0 TSRMLS_CC)


#define MOGILEFS_READ_TIMEOUT 10.0
#define MOGILEFS_CONNECT_TIMEOUT 5.0
#define MOGILEFS_SOCK_STATUS_FAILED 0
#define MOGILEFS_SOCK_STATUS_DISCONNECTED 1
#define MOGILEFS_SOCK_STATUS_UNKNOWN 2
#define MOGILEFS_SOCK_STATUS_CONNECTED 3
/**
 * Compute maximum mogilefs message buffer size
 *
 * (max(keylength) + max(keynamelength) + '=' + '&') * max(listkey_entries) + status + "\r\n")
 */
#define MOGILEFS_MAX_MESSAGE_SIZE ((255 + 7 + 1 + 1) * 1000 + 3 + 2)


/* {{{ struct MogilefsSock */
typedef struct MogilefsSock_ {
	php_stream			*stream;
	char				*host;
	char				*domain;
	unsigned short		port;
	struct timeval		connect_timeout;
	struct timeval		read_timeout;
	long				failed;
	int					status;
} MogilefsSock;
/* }}} */

/* {{{ internal function protos */
PHPAPI int mogilefs_parse_response_to_array(INTERNAL_FUNCTION_PARAMETERS, char *result, int result_len);
PHPAPI MogilefsSock* mogilefs_sock_server_init(char *m_host, int m_host_len, unsigned short m_port, char *m_domain, int m_domain_len, struct timeval timeout);
PHPAPI int mogilefs_sock_connect(MogilefsSock *mogilefs_sock TSRMLS_DC);
PHPAPI int mogilefs_sock_disconnect(MogilefsSock *mogilefs_sock TSRMLS_DC);
PHPAPI int mogilefs_sock_close(MogilefsSock *mogilefs_sock TSRMLS_DC);
PHPAPI int mogilefs_sock_server_open(MogilefsSock *mogilefs_sock, int TSRMLS_DC);
PHPAPI int mogilefs_sock_get(zval *id, MogilefsSock **mogilefs_sock TSRMLS_DC);
PHPAPI int mogilefs_sock_eof(MogilefsSock *mogilefs_sock);
PHPAPI int mogilefs_sock_write(MogilefsSock *mogilefs_sock, char *cmd, unsigned int cmd_len, short free_cmd TSRMLS_DC);
PHPAPI char * mogilefs_sock_read(MogilefsSock *mogilefs_sock, int *buf_len TSRMLS_DC);
PHPAPI char * mogilefs_create_open(MogilefsSock *mogilefs_sock, const char * const key, const char * const class, int multi_dest TSRMLS_DC);
PHPAPI int mogilefs_create_close(MogilefsSock *mogilefs_sock, const char * const m_key, const char * const class, const char * const close_request TSRMLS_DC);
PHPAPI int mogilefs_get_uri_path(const char * const url, php_url **p_url TSRMLS_DC);
PHPAPI void mogilefs_free_socket(MogilefsSock *mogilefs_sock);
/* }}} */

#endif /* PHP_MOGILEFS_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
