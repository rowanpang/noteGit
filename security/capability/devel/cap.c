#include <stdio.h>
#include <sys/capability.h>

typedef struct cap_pangwz_t *cap_tt;

int main(int argc,char** argv)
{
	cap_t caps;
	cap_tt capss;
	
	char* caps_s;
	caps = cap_get_proc();
	caps_s = cap_to_text(caps,NULL);
	printf("1:%s\n",caps_s);	

	cap_value_t cap_list[2];
	cap_list[0]=CAP_FOWNER;
	cap_list[1]=CAP_SETFCAP;

	cap_set_flag(caps,CAP_EFFECTIVE,2,cap_list,CAP_CLEAR);
	if(cap_set_proc(caps) == -1){
		perror("cap_set_proc");
	}
	caps = cap_get_proc();
	caps_s = cap_to_text(caps,NULL);
	printf("2:%s\n",caps_s);	
	
	cap_set_flag(caps,CAP_EFFECTIVE,2,cap_list,CAP_SET);
	if(cap_set_proc(caps) == -1){
		perror("cap_set_proc");
	}
	caps = cap_get_proc();
	caps_s = cap_to_text(caps,NULL);
	printf("3:%s\n",caps_s);	
	
	cap_free(caps); 
	cap_free(caps_s); 
	sleep(40);
	return 0;
}
	
