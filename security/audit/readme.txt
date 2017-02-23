1,ret value
    a,audit_log_user_message 当权限不够时不会返回error.
    b,audit_log_user_avc_message 会返回err.

    c,这是因为libaudit.so中做了处理. ref 源码  
	retval = audit_send_user_message( audit_fd, type, REAL_ERR, buf );
