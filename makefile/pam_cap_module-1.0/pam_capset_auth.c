/* pam_capset_auth module 
 * writen by levysong <songlw@inspur.com> 2013/09/26
 */

#define DEFAULT_USER "nobody"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <linux/prctl.h>
#include <linux/capability.h>


#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD

#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_ext.h>
#include <security/pam_modutil.h>

#include "pam_capset_struct.h"

unsigned long transform_cap_to_int(char *cap_string);

static int set_user_cap(pam_handle_t *pamh,
			user_struct *login_user,
			struct __user_cap_header_struct *cap_user_header_1,
			struct __user_cap_data_struct *cap_user_data_1,
			unsigned long capabilities);

static unsigned long
access_role_config_file(pam_handle_t *pamh, 
			role_struct *role_array[], 
			char *path,
			int role_num);

static int read_user_config_file(pam_handle_t *pamh, 
			user_struct *login_user, 
			role_struct *role_array[],
			char *path);

static int set_diff_user_cap(pam_handle_t *pamh, 
			     struct __user_cap_header_struct *cap_user_header_1,
			     struct __user_cap_data_struct *cap_user_data_1);



PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags ,
		    int argc , const char **argv )
{
	return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh , int flags ,
	       int argc , const char **argv )
{
     return PAM_SUCCESS;
}

/* --- account management functions --- */

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh , int flags ,
		 int argc , const char **argv )
{
     return PAM_SUCCESS;
}

/* --- password management --- */

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh , int flags ,
		 int argc , const char **argv )
{
     return PAM_SUCCESS;
}

/* --- session management --- */

/*
 * set the user's capabilities use capset() call
 * */
static int set_diff_user_cap(pam_handle_t *pamh, 
			     struct __user_cap_header_struct *cap_user_header_1,
			     struct __user_cap_data_struct *cap_user_data_1)
{
	if((capset(cap_user_header_1, cap_user_data_1)) == -1){
		perror("capset");
		pam_syslog(pamh, LOG_INFO, "capset error");
		return -1;
	}
	else{
		capget(cap_user_header_1, cap_user_data_1);
		pam_syslog(pamh, LOG_INFO,"get cap data, effe=0x%x, permit=0x%x, inher=0x%x\n",
			cap_user_data_1->effective, cap_user_data_1->permitted, cap_user_data_1->inheritable);
		return 0;
	}

}

/*
 * set the user's capabilities devide four part: root, secadm, 
 * audadm, guest, other;
 * capabilities: the capabilities to be set;
 * return value: 0: success; -1: error
 */
static int set_user_cap(pam_handle_t *pamh,
			user_struct *login_user,
			struct __user_cap_header_struct *cap_user_header_1,
			struct __user_cap_data_struct *cap_user_data_1,
			unsigned long capabilities)
{
	int loop_time;
	
	if(strcmp(login_user->user_name, "root") == 0)
		goto root_cap_set;
	else if(strcmp(login_user->user_name, "secadm") == 0)
		goto secadm_cap_set;
	else if(strcmp(login_user->user_name, "audadm") == 0)
		goto audadm_cap_set;
	else if(strcmp(login_user->user_name, "guest") == 0)
		goto guest_cap_set;
	else
		goto other_cap_set;

root_cap_set:

	/*
 	 * the root capbilities was not allowed to modify,
 	 * so we assign it the minimum capbilities 0x2611000 
 	 */

#if 0
	capabilities |= (unsigned long)(1) << CAP_AUDIT_WRITE;
	cap_user_data_1->effective = capabilities;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = capabilities;

	/*0xffffffff used to develop*/
/*
	cap_user_data_1->effective = 0xffffffff;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = 0xffffffff;
*/

#else
	cap_user_data_1->effective = 0x22613000;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = 0x22613000;
#endif

#if 1
	for(loop_time = 0; loop_time < 0x22; loop_time++){
		if(((cap_user_data_1->permitted >> loop_time) & 1) == 1)
			continue;

		if(prctl(PR_CAPBSET_DROP, loop_time) != 0){
			pam_syslog(pamh, LOG_INFO, "prctl error");
			return -1;
		}
	}
#endif
	return set_diff_user_cap(pamh, cap_user_header_1, cap_user_data_1);

secadm_cap_set:

	/*
 	 * the secadm capbilities was not allowed to modify,
 	 * so we assign it the minimum capbilities 0x62611000 
 	 */
#if 0

	capabilities |= (unsigned long)(1) << CAP_AUDIT_WRITE;
	cap_user_data_1->effective = capabilities;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = capabilities;
#else
	cap_user_data_1->effective = 0x200000c0;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = 0x200000c0;
#endif

	return set_diff_user_cap(pamh, cap_user_header_1, cap_user_data_1);

audadm_cap_set:

	/*
 	 * the audadm capbilities was not allowed to modify,
 	 * so we assign it the minimum capbilities 0x600000c0,
 	 * in kernel we clear the CAP_SETUID and CAP_SETGID;
 	 * at the end of the login capability set is 0x60000000 
 	 */
#if 1
	cap_user_data_1->effective = 0x600000c0;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = 0x600000c0;
#else 
	capabilities |= ( ((unsigned long)(1) << CAP_SETUID) |
			 (((unsigned long)(1) << CAP_AUDIT_WRITE)));
	cap_user_data_1->effective = capabilities;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = capabilities;
#endif

	return set_diff_user_cap(pamh, cap_user_header_1, cap_user_data_1);

guest_cap_set:
other_cap_set:
	/*
 	 * the guest user's and other user's capbilities was set to 0,
 	 */
#if 1
	/*give the setuid and setgid cap, after this this two cap will be clear*/
	capabilities |= (((unsigned long)(1) << CAP_AUDIT_WRITE) |
			 ((unsigned long)(1) << CAP_SETUID) |
			 (((unsigned long)(1) << CAP_AUDIT_WRITE)));
	cap_user_data_1->effective = capabilities;
	cap_user_data_1->permitted = 0xffffffff;
	cap_user_data_1->inheritable = capabilities;
#else
	cap_user_data_1->effective = 0;
	cap_user_data_1->effective = 0;
	cap_user_data_1->permitted = 0;
#endif

	return set_diff_user_cap(pamh, cap_user_header_1, cap_user_data_1);
}

/*
 * use to read user.conf file
 * return value 0:success  -1:error  >0:role number
 */
static int read_user_config_file(pam_handle_t *pamh, 
			user_struct *login_user, 
			role_struct *role_array[],
			char *path)
{
	FILE *fp;
	int retval;
	int lineno;
	int end;
	char *sptr;
	char *seg_ptr;
	char line[BUF_SIZE];
	int role_num = 0;
	int compare_val = 1;

	/*
 	 * read the user.conf and role.conf 
 	 */
#if 1
	if((fp = fopen(path, "r")) != NULL){

		while(fgets(line, sizeof(line), fp) && compare_val){
			lineno++;

			if(line[end = strlen(line) - 1] != '\n'){
				pam_syslog(pamh, LOG_INFO, "%s: line %d:missing new line or line too long\n",
						path, lineno);
				continue;
			}

			if(line[0] == '#')	/*comment line*/
				continue;

			while(end > 0 && isspace(line[end - 1])) /*move the space at the end of the line*/
				end--;
		//	line[end] = 0;

			if(line[0] == 0)	/*skip blank line*/
				continue;

		/*get the user name in the user.conf*/
			seg_ptr = strtok_r(line, ";,", &sptr);
			if(strcmp(seg_ptr, login_user->user_name) == 0){
				seg_ptr = strtok_r(NULL, ";,", &sptr);	/*get the user's uid in user.conf*/
				login_user->user_id = atoi(seg_ptr);

				while(strcmp(seg_ptr, "\n")){
					seg_ptr = strtok_r(NULL, ";,", &sptr);	/*get the role allocated to other user*/
					if(strcmp(seg_ptr, "\n") == 0)
						break;

					if((strcmp(seg_ptr, "sysadm_r") == 0 ||
					   strcmp(seg_ptr, "secadm_r") == 0 ||
					   strcmp(seg_ptr, "auditadm_r") == 0) &&
					   (strcmp(login_user->user_name, "root") &&
					    strcmp(login_user->user_name, "secadm") &&
					    strcmp(login_user->user_name, "audadm"))){
						pam_syslog(pamh, LOG_INFO, "user %s is not permitted to own the %s role\n",
								login_user->user_name, seg_ptr);
						fclose(fp);
						return -1;
					}
						strcpy(role_array[role_num++]->role_name, seg_ptr);
					/*	pam_syslog(pamh, LOG_INFO, "role_array[%d]->role_name = %s\n", role_num-1,  role_array[role_num-1]->role_name);*/
				}
			}
		}/*while(fgets()) end*/

	}/*if(fopen(), "r") end*/
	else{
		perror("fopen");	
		pam_syslog(pamh, LOG_INFO, "fopen error\n");
		fclose(fp);
		return -1;
	}

	fclose(fp);
	
#endif
	return role_num;
}

/*
 * transform the string capability to integer
 * return the integer capability
 * if capability do not exist return -1
 */
unsigned long transform_cap_to_int(char *cap_string)
{
	int i;
	char *cap_str[36] = {"CAP_CHOWN", 
				"CAP_DAC_OVERRIDE", "CAP_DAC_READ_SEARCH", "CAP_FOWNER", "CAP_FSETID","CAP_KILL", 
				"CAP_SETGID", "CAP_SETUID", "CAP_SETPCAP", "CAP_LINUX_IMMUTABLE", "CAP_NET_BIND_SERVICE",
				"CAP_NET_BROADCAST", "CAP_NET_ADMIN", "CAP_NET_RAW", "CAP_IPC_LOCK", "CAP_IPC_OWNER",
				"CAP_SYS_MODULE", "CAP_SYS_RAWIO", "CAP_SYS_CHROOT", "CAP_SYS_PTRACE", "CAP_SYS_PACCT",
				"CAP_SYS_ADMIN", "CAP_SYS_BOOT", "CAP_SYS_NICE", "CAP_SYS_RESOURCE", "CAP_SYS_TIME",
				"CAP_SYS_TTY_CONFIG", "CAP_MKNOD", "CAP_LEASE", "CAP_AUDIT_WRITE", "CAP_AUDIT_CONTROL",
				"CAP_SETFCAP", "CAP_MAC_OVERRIDE", "CAP_MAC_ADMIN", 
				NULL, NULL};
	for(i = 0; i < 34; i++){
		if(strcmp(cap_string, cap_str[i]) == 0)
			return i;
	}
	
	if(i == 34){
	//	pam_syslog(pamh, LOG_INFO, "kernel don't have the capability %s", cap_string);
		return -1;
	}
}

/* 
 * in order to get the capability in the role.conf
 * role_array: role_struct;
 * role_num: the number of the role
 * return -1 error; >0 the capability; 0 no capability 
 */
static unsigned long
access_role_config_file(pam_handle_t *pamh, 
			role_struct *role_array[], 
			char *path,
			int role_num)
{
	FILE *fp;
	int retval;
	int end;
	int i;
	char *seg_ptr;
	char *sptr;
	char line[BUF_SIZE];
	unsigned long cap_val = 0;
	int lineno = 0;
	int compare_val = 1;
	
	
	if((fp = fopen(path, "r")) != NULL){
		while(fgets(line, sizeof(line), fp)){
			lineno++;

			if(line[end = strlen(line) -1] != '\n'){
				pam_syslog(pamh, LOG_INFO, "%s: line %d:missing new line or line too long\n",
						path, lineno);
				continue;
			}
			
			if(line[0] == '#')
				continue;
			
			while(end > 0 && isspace(line[end -1]))
				end--;
		//	line[end] = 0;
		
			if(line[0] == 0)
				continue;
		
			/*get the role name*/
			seg_ptr = strtok_r(line, ";,", &sptr);

			/*compare if seg_ptr is the role in the role_array*/
			for(i = 0; i < role_num; i++){
				if(strcmp(seg_ptr, role_array[i]->role_name) == 0){
					/*this segment is the role_id*/
					seg_ptr = strtok_r(NULL, ";,", &sptr);
					role_array[i]->role_id = atoi(seg_ptr);

					while(strcmp((seg_ptr = strtok_r(NULL, ";,", &sptr)), "\n")){
						/*transform the capability into integer*/
						if((retval = transform_cap_to_int(seg_ptr)) == -1)
							//return 0;
							continue;
						role_array[i]->role_cap = role_array[i]->role_cap | ((unsigned long)1 << retval);
						cap_val = cap_val | ((unsigned long)1 << retval);
					/*	pam_syslog(pamh, LOG_INFO, "debug info: cap_val = 0x%llx, retval = 0x%llx, capability = %s\n", 
								cap_val, (unsigned long)1<<retval, seg_ptr);*/
		
					}
				}
				
			}
			
			/* 
 			 * because current line role name was not allocated to the user
 			 * so continue read new line till the end of the file;
 			 * */
			continue;
		}
		
		/*
 		 * because role.conf do not have the role in the role_array,
 		 * so return 0 as the capability of the user
 		 * */
		fclose(fp);
		return cap_val;
	}
	else{
		pam_syslog(pamh, LOG_INFO, "open file %s error\n", path);
		fclose(fp);
		return -1;
	}
	
	return cap_val;
}


PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh , int flags ,
		    int argc , const char **argv )
{
	FILE *fp;
	int retval;
	int role_num;
	int compare_val = 1;
	int lineno = 0;
	int end;
	int i, j;
	unsigned long loop_time;
	unsigned long capabilities_val;
	const char *user=NULL;
	char *sptr;
	char *seg_ptr;
	char line[BUF_SIZE];
	struct __user_cap_header_struct *cap_user_header_1;
	struct __user_cap_data_struct *cap_user_data_1;
	user_struct *login_user;
	role_struct *login_user_role;
	role_struct *role_array[8];

	cap_user_header_1 = (struct __user_cap_header_struct *)malloc(sizeof(struct __user_cap_header_struct));
	cap_user_data_1 = (struct __user_cap_data_struct *)malloc(sizeof(struct __user_cap_data_struct));
	login_user = (user_struct *)malloc(sizeof(user_struct));
	login_user_role = (role_struct *)malloc(sizeof(role_struct));

	for(i = 0; i < 8; i++){
		role_array[i] = (role_struct *)malloc(sizeof(role_struct));
		memset(role_array[i]->role_name, 0, NAME_MAX_LEN);
		role_array[i]->role_id = 0;
		role_array[i]->role_cap = 0;
	}

	cap_user_header_1->version = _LINUX_CAPABILITY_VERSION_1;
	cap_user_header_1->pid = getpid();

	/*
	 * authentication requires we know who the user wants to be
	 */
	retval = pam_get_item(pamh, PAM_USER, (void*)&user);
	if (user == NULL || *user == '\0' || retval != PAM_SUCCESS) {
		pam_syslog(pamh, LOG_INFO, "username not known\n");
		return PAM_AUTH_ERR;
	}
	/*store the user name in login_user*/
	strcpy(login_user->user_name, user);

	role_num = read_user_config_file(pamh, login_user, role_array, USER_CONF_FILE);
	if(role_num == -1)
		return PAM_AUTH_ERR;
	/*
 	 * if user do not have role, login was permited
 	 */
	if(role_num == 0){
		pam_syslog(pamh, LOG_INFO, "secadm do not allowed user:%s to login the system.\n", login_user);
		return PAM_AUTH_ERR;
	}

#if 1
	if((capabilities_val = access_role_config_file(pamh, role_array, ROLE_CONF_FILE, role_num)) > 0){
		pam_syslog(pamh, LOG_INFO, "all capabilities = 0x%lX\n", capabilities_val );
		for(j = 0; j < role_num; j++)
			pam_syslog(pamh, LOG_INFO, "role[%d]: name-->%s, id-->%d, cap->0x%LX\n", 
						j, role_array[j]->role_name, role_array[j]->role_id, role_array[j]->role_cap);
	}
	else if(capabilities_val == 0)
		pam_syslog(pamh, LOG_INFO, "have no such cap\n");
	else 
		pam_syslog(pamh, LOG_INFO, "error happend\n");

	if (set_user_cap(pamh, login_user, cap_user_header_1, cap_user_data_1, capabilities_val) != 0)
		pam_syslog(pamh, LOG_INFO, "set_user_cap error\n");
#endif
	free(cap_user_header_1);
	free(cap_user_data_1);
	free(login_user);
	free(login_user_role);
	for(i = 0; i < 8; i++)
		free(role_array[i]);

	return PAM_SUCCESS;

}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh , int flags ,
		     int argc, const char **argv )
{
     return PAM_SUCCESS;
}

/* end of module definition */

#ifdef PAM_STATIC

/* static module data */

struct pam_module _pam_permit_modstruct = {
    "pam_capset_auth",
    pam_sm_authenticate,
    pam_sm_setcred,
    pam_sm_acct_mgmt,
    pam_sm_open_session,
    pam_sm_close_session,
    pam_sm_chauthtok
};

#endif
