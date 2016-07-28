#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <unistd.h>

#define DBUSNAME "test.signal.receiver"


DBusConnection* make_connection(void)
{
	DBusConnection * con;  
	DBusError err;  
	int ret;  

	//1,make a connection with dbus-daemon
	dbus_error_init(&err);
	con = dbus_bus_get(DBUS_BUS_SESSION,&err);
	if(dbus_error_is_set(&err)){
		fprintf(stderr,"ConnectionError %s\n",err.message);  
        dbus_error_free(&err); 
		con = NULL;
		goto OUT;
	}
	
	if(con == NULL){
		goto OUT;
	}	
	
	//2,request a well-known name
	ret = dbus_bus_request_name(con,DBUSNAME,DBUS_NAME_FLAG_REPLACE_EXISTING,&err);
	if(dbus_error_is_set(&err)){  
        fprintf(stderr,"Name Error%s\n",err.message);  
        dbus_error_free(&err);  
		con = NULL;
		goto OUT;
    }  
    if(ret !=DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER){
		con = NULL;
		goto OUT;
	}

OUT:
	return con;
}


int poll_signal(DBusConnection *con)
{
	DBusError err;  
	DBusMessage * msg;
	DBusMessageIter argIter;
    int ret;  
	char *sigvalue;

	dbus_bus_add_match(con,"type='signal',interface='test.signal.sender.signal.type'",&err);
	dbus_connection_flush(con);

	if(dbus_error_is_set(&err)){
		fprintf(stderr,"Match Error%s\n",err.message);  
        dbus_error_free(&err);  
		ret = -1;
		goto OUT;
    }  	
	
	while(1){
		printf("poll signal ----\n");
		dbus_connection_read_write(con,0);
		msg = dbus_connection_pop_message(con);	
		if(msg == NULL){
			sleep(1);
			continue;
		}
		
		if(dbus_message_is_signal(msg,"test.signal.sender.signal.type","Test")){
			if(!dbus_message_iter_init(msg,&argIter)){
				fprintf(stderr,"MessageHas no Param");  
			}else if(dbus_message_iter_get_arg_type(&argIter)!= DBUS_TYPE_STRING){
				fprintf(stderr,"not a string"); 	
			}else{
				dbus_message_iter_get_basic(&argIter,&sigvalue); 
			}		
			printf("Got signal:%s with value:%s\n","Test",sigvalue);
		}
		dbus_message_unref(msg);
	}	

	ret = 0;
OUT:
	return ret;
}

int main(int argc,char** argv)
{
	DBusConnection *con;
	int ret;
	con = make_connection();
	ret = poll_signal(con);

	return ret;
}
	
