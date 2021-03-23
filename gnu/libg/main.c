#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <glib.h>

typedef struct _DemoSource DemoSource;
struct _DemoSource
{
    GSource         source;
    gint64          interval;
    GTimeVal        last;
};

DemoSource *s1, *s2;
const char* cur_source_name = NULL;

static glong my_time_val_difference (const GTimeVal *compare, const GTimeVal *now)
{
    return (compare->tv_sec - now->tv_sec) * 1000 + (compare->tv_usec - now->tv_usec) / 1000;
}

static const char* get_source_str( GSource* source_ )
{
    if(source_ == &(s1->source)){
        return "s1";
    }else if(source_ == &(s2->source)){
        return "s2";
    }

    return "unkown";
}

static gboolean prepare_func( GSource *source_, gint *timeout )
{
    DemoSource* source = (DemoSource*)source_;
    GTimeVal now,target;
    glong diff;
    gint64 us;

    cur_source_name = get_source_str(source_);

    us=g_source_get_time(source_);
    g_time_val_add(&now,us);

    target = source->last;
    g_time_val_add(&target,source->interval);

    diff = my_time_val_difference(&now,&target);

    printf("-----%s Enter,Source:%s -----\n", __FUNCTION__,cur_source_name);

    if(source_ == &(s1->source)){
        *timeout = 2*1000;
    }else if(source_ == &(s2->source)){
        *timeout = 4*1000;
    }

    return FALSE;


    if( diff >= 0 ) {
        *timeout = 0;
        printf( "Source %s need to dispatch right now. D:%d\n", cur_source_name, diff );
        return TRUE;
    } else {
        *timeout = -diff;
        printf( "Source %s need to sleep %d ms.\n", cur_source_name, *timeout );
        return FALSE;
    }
    printf("-----%s Exit-----\n", __FUNCTION__);
}

static gboolean check_func( GSource* source_ )
{
    DemoSource* source = (DemoSource*)source_;
    GTimeVal now, target;
    glong diff;
    gint64 us;

    cur_source_name = get_source_str( source_ );

    us = g_source_get_time(source_);
    g_time_val_add(&now, us);

    target = source->last;
    g_time_val_add( &target, source->interval );

    diff = my_time_val_difference( &now, &target );

    printf("-----%s Enter -----\n", __FUNCTION__);
    if(diff >= 0) {
        printf("Yes. Source %s need to dispatch.\n", cur_source_name );
        return TRUE;
    } else {
        printf("No. Source %s want to sleep.\n", cur_source_name );
        return FALSE;
    }

    printf("-----%s Exit-----\n", __FUNCTION__);
}

static gboolean dispatch_func(GSource *source_, GSourceFunc callback, gpointer user_data)
{
        cur_source_name = get_source_str( source_ );
        printf("-----%s Enter -----\n", __FUNCTION__);
        DemoSource* source = (DemoSource*)source_;
        g_get_current_time( &source->last );
        printf("Source %s dispatched.\n", cur_source_name);
        printf("-----%s Exit-----\n", __FUNCTION__);
}

static void finalize_func( GSource* source_ )
{
        cur_source_name = get_source_str( source_ );
        printf("-----%s Enter -----\n", __FUNCTION__);
        printf("Source %s finalized.\n", cur_source_name );
        printf("-----%s Exit-----\n", __FUNCTION__);
}

GSourceFuncs source_funcs =
{
        prepare_func,
        check_func,
        dispatch_func,
        finalize_func,
};

int main( int argc, char** argv )
{
    GMainLoop* loop;

    loop = g_main_loop_new( NULL, TRUE );

    s1 = (DemoSource*)g_source_new( &source_funcs, sizeof(DemoSource) );
    s2 = (DemoSource*)g_source_new( &source_funcs, sizeof(DemoSource) );
    s1->interval = 2*1000*1000;             //us
    s2->interval = 3*1000*1000;             //us

    g_get_current_time( &s1->last );
    g_get_current_time( &s2->last );

    g_source_attach( (GSource*)s1, NULL );
    g_source_attach( (GSource*)s2, NULL );

    printf( "s1 addr:0x%p\n", s1);
    printf( "s2 addr:0x%p\n", s2);

    g_main_loop_run( loop );

    return 0;
}
