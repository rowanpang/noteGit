#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
/*#include <dbus/dbus-glib.h>*/
#include <dbus/dbus.h>
#include <unistd.h>

#define DBUSNAME "test.signal.sender"
#define SENDEROBJ "/test/signal/sender/signal"
#define SENDEROBJIF "test.signal.sender.signal.type"
#define SIGNALNAME "Test"
DBusConnection* makeConnect(void)
{
	DBusError err;
	DBusConnection* con;
	int ret ;

	dbus_error_init(&err);
	//fir:make connection with the dbus-daemon
	con = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if(dbus_error_is_set(&err)){
		fprintf(stderr,"ConnectionErr: %s\n",err.message);
		con = NULL;
		dbus_error_free(&err);
		goto OUT;
	}

	//sec:got a well-known name
	ret = dbus_bus_request_name(con,DBUSNAME,DBUS_NAME_FLAG_REPLACE_EXISTING,&err);
	if(dbus_error_is_set(&err)){
		fprintf(stderr,"request well-known name err: %s\n",err.message);
		dbus_error_free(&err);
	}
	if(ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER){
		con = NULL;
		goto OUT;
	}

OUT:
	return con;
}
		
int sendmsg(DBusConnection *con, char *sigvalue)
{
	DBusMessage * msg;  
	DBusMessageIter arg;
	DBusError err;
	int ret;
	//3,alloc a msg
	msg = dbus_message_new_signal(SENDEROBJ,SENDEROBJIF,SIGNALNAME);
	if(msg == NULL){
		fprintf(stderr,"MessageNULL\n");  
		ret = -1;
		goto OUT;
	}

	//4,msg args
	dbus_message_iter_init_append(msg,&arg);
	ret = dbus_message_iter_append_basic(&arg,DBUS_TYPE_STRING,&sigvalue);
	if(!ret){
		ret = -2;
		fprintf(stderr,"MessageNULL\n");  
		goto OUT;
	}

	//5,send it
	ret = dbus_connection_send(con,msg,NULL);
	if(!ret){
		ret = -3;
		fprintf(stderr,"send error\n");  
		goto OUT;
	}

	//6,flush
	dbus_connection_flush(con);

	//7,release
	dbus_message_unref(msg);	

	ret = 0;
OUT:
	return ret;
}


	

int main(int argc,char **argv)
{
	DBusConnection* con;
	int ret;
	con = makeConnect();
	if(con == NULL){
		ret = -1;
		fprintf(stderr,"makeconnection error\n");
		goto OUT;
	}

	ret = sendmsg(con,"hello dbus\n");
	if(ret){
		ret = -2;
		fprintf(stderr,"msg send error\n");
		goto OUT;
	}

	fprintf(stdout,"-------^_^--------msg send ok !!!\n");
	ret = 0;
OUT:
	return ret;
}
