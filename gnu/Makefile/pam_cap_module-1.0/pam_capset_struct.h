/* pam_capset_auth module header file
 * define the constants and structuers for the pam_capset_auth module
 * wirten by levysong <songlw@inspur.com> 2013/10/17
 */

/*the role id*/
#define	SYSADM_ROLE_ID		(0)
#define	SECADM_ROLE_ID		(1)
#define	AUDITADM_ROLE_ID	(2)
#define	GUEST_ROLE_ID		(3)
#define	MIN_USER_ROLE_ID	(4)

/*configure file*/
#define	USER_CONF_FILE		"/etc/security/conf/user.conf"
#define	ROLE_CONF_FILE		"/etc/security/conf/role.conf"

/*limit value for user name or buffer name */
#define NAME_MAX_LEN		(256)
#define BUF_SIZE		(1024)

/*the struct to store the user id, role id*/
typedef struct __user_struct_t {
	char user_name[NAME_MAX_LEN];
	int user_id;
	char user_role[BUF_SIZE];
}user_struct;

typedef struct __role_struct_t {
	char role_name[NAME_MAX_LEN];
	int role_id;
	unsigned long role_cap;
	//char role_cap[BUF_SIZE];
}role_struct;

