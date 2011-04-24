#ifdef RCSID
static char RCSid[] =
"$Header$";
#endif

/* Copyright (c) 2005 by Michael J. Roberts.  All Rights Reserved. */
/*
Name
  vmbifnet.cpp - TADS networking function set
Function
  
Notes
  
Modified
  04/20/2010 MJRoberts  - Creation
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "t3std.h"
#include "os.h"
#include "utf8.h"
#include "charmap.h"
#include "vmbifnet.h"
#include "vmstack.h"
#include "vmerr.h"
#include "vmerrnum.h"
#include "vmglob.h"
#include "vmpool.h"
#include "vmobj.h"
#include "vmstr.h"
#include "vmlst.h"
#include "vmrun.h"
#include "vmhost.h"
#include "vmconsol.h"
#include "vmvec.h"
#include "vmnet.h"
#include "vmimport.h"
#include "vmpredef.h"
#include "vmhttpsrv.h"


/* ------------------------------------------------------------------------ */
/*
 *   NetEvent.evType values 
 */
#define VMBN_EVT_REQUEST    1
#define VMBN_EVT_TIMEOUT    2
#define VMBN_EVT_DBGBRK     3


/* ------------------------------------------------------------------------ */
/*
 *   Static initialization 
 */
void CVmBifNet::attach(VMG0_)
{
    /* initialize the network layer */
    os_net_init();

    /* create the network message queue */
    G_net_queue = new TadsMessageQueue(0);
}

/*
 *   Static cleanup 
 */
void CVmBifNet::detach(VMG0_)
{
    /* done with the message queue - flush it and release our reference */
    G_net_queue->flush();
    G_net_queue->release_ref();
    G_net_queue = 0;
}

/* ------------------------------------------------------------------------ */
/*
 *   Connect to the Web UI
 */
void CVmBifNet::connect_ui(VMG_ uint oargc)
{
    /* check arguments */
    check_argc(vmg_ oargc, 2);

    /* get the server object */
    vm_val_t *srv = G_stk->get(0);
    if (srv->typ != VM_OBJ
        || !CVmObjHTTPServer::is_httpsrv_obj(vmg_ srv->val.obj))
        err_throw(VMERR_BAD_TYPE_BIF);

    /* get the object pointer properly cast */
    CVmObjHTTPServer *srvp = (CVmObjHTTPServer *)vm_objp(vmg_ srv->val.obj);

    /* get the URL path */
    const char *path = G_stk->get(1)->get_as_string(vmg0_);
    if (path == 0)
        err_throw(VMERR_BAD_TYPE_BIF);

    /* make a null-terminated copy of the path */
    char *pathb = lib_copy_str(path + VMB_LEN, vmb_get_len(path));

    /* get the server network address information */
    char *errmsg = 0;
    char *addr = 0, *ip = 0;
    int port;
    int ok = srvp->get_listener_addr(addr, ip, port);

    /* 
     *   If we don't have a network configuration yet, create one.  This
     *   notifies other subsystems that we have an active web UI; for
     *   example, the presence of a network UI disables the regular console
     *   UI, since all UI actions have to go through the web UI once it's
     *   established.
     *   
     *   The interpreter startup creates a network configuration if it
     *   receives command-line information telling it that the game was
     *   launched by a Web server in response to an incoming client request.
     *   When the user launches the game in stand-alone mode directly from
     *   the operating system shell, there's no Web server launch
     *   information, so the startup code doesn't create a net config object.
     *   So, if we get here and find we don't have this object, it means that
     *   we're running in local standalone mode.  
     */
    if (G_net_config == 0)
        G_net_config = new TadsNetConfig();

    /* connect */
    if (ok)
    {
        /* connect */
        ok = osnet_connect_webui(vmg_ addr, port, pathb, &errmsg);
    }
    else
    {
        /* couldn't get the server address */
        errmsg = lib_copy_str(
            "No address information available for HTTPServer");
    }

    /* free strings */
    lib_free_str(pathb);
    lib_free_str(addr);
    lib_free_str(ip);

    /* if there's an error, throw it */
    if (!ok)
    {
        G_interpreter->push_string(vmg_ errmsg);
        lib_free_str(errmsg);
        G_interpreter->throw_new_class(vmg_ G_predef->net_exception, 1,
                                       "Error connecting to Web UI");
    }

    /* no return value */
    retval_nil(vmg0_);
}

/* ------------------------------------------------------------------------ */
/*
 *   Get a network event
 */
void CVmBifNet::get_event(VMG_ uint oargc)
{
    /* check arguments */
    check_argc_range(vmg_ oargc, 0, 1);

    /* get the timeout, if present */
    long timeout = OS_FOREVER;
    if (oargc >= 1)
    {
        if (G_stk->get(0)->typ == VM_NIL)
            G_stk->discard();
        else
            timeout = pop_int_val(vmg0_);
    }

    /* presume the return value will be a base NetEvent object */
    vm_obj_id_t evt_cl = G_predef->net_event;

    /* get a pointer to the result object */
    CVmObject *evt_obj = vm_objp(vmg_ G_interpreter->get_r0()->val.obj);

    /* we don't know the NetEvent subclass yet, so presume the base class */
    vm_obj_id_t ev_cl = G_predef->net_event;
    int argc;

    /* get the next message */
    TadsMessage *msg = 0;
    int wret = G_net_queue->wait(vmg_ timeout, &msg);

    /* cast the message */
    TadsEventMessage *evtmsg = cast_tads_message(TadsEventMessage, msg);

    /* make sure we release the message before exiting */
    err_try
    {
        /* check the wait result */
        const char *err;
        switch (wret)
        {
        case OSWAIT_EVENT + 0:
            /* we got a message */
            if (evtmsg != 0)
            {
                /* ask the message to set up a new NetRequestEvent */
                int evt_code;
                ev_cl = evtmsg->prep_event_obj(vmg_ &argc, &evt_code);
            
                /* add the event type argument */
                G_interpreter->push_int(vmg_ evt_code);
                ++argc;
            }
            else
            {
                /* unrecognized message type */
                err = "getNetEvent(): unexpected message type in queue";
                goto evt_error;
            }
            break;

        case OSWAIT_EVENT + 1:
            /* 'quit' event - return an error */
            err = "getNetEvent(): queue terminated";
            goto evt_error;

        case OSWAIT_EVENT + 2:
            /* debug break - return a NetEvent with the interrupt code */
            argc = 1;
            G_interpreter->push_int(vmg_ VMBN_EVT_DBGBRK);
            break;
            
        case OSWAIT_TIMEOUT:
            /* the timeout expired - return a NetTimeoutEvent */
            ev_cl = G_predef->net_timeout_event;
            argc = 1;
            G_interpreter->push_int(vmg_ VMBN_EVT_TIMEOUT);
            break;

        case OSWAIT_ERROR:
        default:
            /* the wait failed */
            err = "getNetEvent(): error waiting for request message";

        evt_error:
            /* on error, throw a NetException describing the problem */
            G_interpreter->push_string(vmg_ err);
            G_interpreter->throw_new_class(
                vmg_ G_predef->net_exception, 1, err);
            AFTER_ERR_THROW(break;)
        }
        
        /* 
         *   if the specific event type subclass isn't defined, fall back on
         *   the base NetEvent class 
         */
        if (ev_cl == VM_INVALID_OBJ)
            ev_cl = G_predef->net_event;

        /* 
         *   construct the NetEvent subclass, or a generic list if we don't
         *   even have the NetEvent class defined 
         */
        if (ev_cl != VM_INVALID_OBJ)
            vm_objp(vmg_ ev_cl)->create_instance(vmg_ ev_cl, 0, argc);
        else
            retval_obj(vmg_ CVmObjList::create_from_stack(vmg_ 0, argc));
    }
    err_finally
    {
        /* we're done with the message object */
        if (msg != 0)
            msg->release_ref();
    }
    err_end;
}

/* ------------------------------------------------------------------------ */
/*
 *   Get the local host name
 */
void CVmBifNet::get_hostname(VMG_ uint oargc)
{
    /* check arguments */
    check_argc(vmg_ oargc, 0);

    /* check the network safety levels */
    int client_level, server_level;
    G_host_ifc->get_net_safety(&client_level, &server_level);

    /* 
     *   If the network safety level doesn't allow outside network access for
     *   either client or server, return "localhost".  If they don't allow
     *   any network access at all, return nil.
     *   
     *   If there's a host name defined in the web configuration, return that
     *   host name.  Otherwise, return the default host name from the OS.  
     */
    if (client_level >= VM_NET_SAFETY_MAXIMUM
        && server_level >= VM_NET_SAFETY_MAXIMUM)
    {
        /* no network access is allowed - return nil */
        retval_nil(vmg0_);
    }
    else if (client_level >= VM_NET_SAFETY_LOCALHOST
        && server_level >= VM_NET_SAFETY_LOCALHOST)
    {
        /* localhost access only - return "localhost" */
        retval_str(vmg_ "localhost");
    }
    else
    {
        /* 
         *   the network safety level allows outside network access, so we
         *   can return the actual host name - get it from the system
         */
        char buf[256];
        if (os_get_hostname(buf, sizeof(buf)))
        {
            /* got it - return the string */
            retval_ui_str(vmg_ buf);
        }
        else
        {
            /* no host name available - return nil */
            retval_nil(vmg0_);
        }
    }
}

/* ------------------------------------------------------------------------ */
/*
 *   Get the local host IP address
 */
void CVmBifNet::get_host_ip(VMG_ uint oargc)
{
    /* check arguments */
    check_argc(vmg_ oargc, 0);

    /* check the network safety levels */
    int client_level, server_level;
    G_host_ifc->get_net_safety(&client_level, &server_level);

    /* 
     *   If the network safety level doesn't allow outside network access for
     *   either client or server, return "localhost".  If they don't allow
     *   any network access at all, return nil.  
     */
    if (client_level >= VM_NET_SAFETY_MAXIMUM
        && server_level >= VM_NET_SAFETY_MAXIMUM)
    {
        /* no network access is allowed - return nil */
        retval_nil(vmg0_);
    }
    else if (client_level >= VM_NET_SAFETY_LOCALHOST
             && server_level >= VM_NET_SAFETY_LOCALHOST)
    {
        /* localhost access only - return the standard localhost IP address */
        retval_str(vmg_ "127.0.0.1");
    }
    else
    {
        /* retrieve the host IP address for the default host */
        char buf[256];
        if (os_get_local_ip(buf, sizeof(buf), 0))
        {
            /* got it - return the string */
            retval_ui_str(vmg_ buf);
        }
        else
        {
            /* no host name available - return nil */
            retval_nil(vmg0_);
        }
    }
}


/* ------------------------------------------------------------------------ */
/*
 *   Get the network storage server URL. 
 */

/* get the storage server URL */
void CVmBifNet::get_storage_url(VMG_ uint oargc)
{
    /* check arguments */
    check_argc(vmg_ oargc, 1);

    /* set a default nil return in case we can't build the path */
    retval_nil(vmg0_);

    /* get the resource name */
    const char *page = G_stk->get(0)->get_as_string(vmg0_);
    if (page == 0)
        err_throw(VMERR_STRING_VAL_REQD);

    /* get the resource name length and buffer pointer */
    size_t pagelen = vmb_get_len(page);
    page += VMB_LEN;

    /* if there's a network configuration, build the resource path */
    const char *host = 0, *rootpath = 0;
    if (G_net_config != 0)
    {
        /* get the storage server host name and root path */
        host = G_net_config->get("storage.domain");
        rootpath = G_net_config->get("storage.rootpath", "/");
    }

    /* we must have a host name to proceed */
    if (host != 0)
    {
        /* build the full string */
        G_interpreter->push_stringf(vmg_ "http://%s%s%.*s",
                                    host, rootpath,
                                    (int)pagelen, page);

        /* pop it into R0 */
        G_stk->pop(G_interpreter->get_r0());
    }

    /* discard arguments */
    G_stk->discard();
}


/* ------------------------------------------------------------------------ */
/*
 *   Get the launching host address
 */
void CVmBifNet::get_launch_host_addr(VMG_ uint oargc)
{
    /* check arguments */
    check_argc(vmg_ oargc, 0);

    /* get the launch host name from the network configuration */
    const char *host = (G_net_config != 0
                        ? G_net_config->get("hostname")
                        : 0);

    /* if there's a host name, return it, otherwise return nil */
    if (host != 0)
        retval_str(vmg_ host);
    else
        retval_nil(vmg0_);
}

/* ------------------------------------------------------------------------ */
/*
 *   Get a configuration variable
 */
#if 0 // leave this out for now
void CVmBifNet::get_net_config(VMG_ uint oargc)
{
    /* check arguments */
    check_argc(vmg_ oargc, 1);

    /* get the variable name */
    char name[256];
    pop_str_val_buf(vmg_ name, sizeof(name));

    /* look up the name */
    const char *val = 0;
    if (G_net_config != 0)
        val = G_net_config->get(name);

    /* if we found a value, return it; otherwise return nil */
    if (val != 0)
        retval_str(vmg_ val);
    else
        retval_nil(vmg0_);
}
#endif
