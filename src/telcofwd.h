#ifndef TELCOQML_TELCOFWD_H
#define TELCOQML_TELCOFWD_H

// FIXME: Investigate compatibility issue between Qt and Telco headers.

extern "C"
{
    typedef struct _TelcoApplication TelcoApplication;
    typedef struct _TelcoCrash TelcoCrash;
    typedef struct _TelcoDevice TelcoDevice;
    typedef struct _TelcoDeviceManager TelcoDeviceManager;
    typedef struct _TelcoIcon TelcoIcon;
    typedef struct _TelcoProcess TelcoProcess;
    typedef struct _TelcoScript TelcoScript;
    typedef struct _TelcoSession TelcoSession;
    typedef struct _TelcoSpawnOptions TelcoSpawnOptions;

    typedef struct _GAsyncResult GAsyncResult;
    typedef struct _GBytes GBytes;
    typedef struct _GError GError;
    typedef struct _GObject GObject;
    typedef struct _GSource GSource;

    typedef void *gpointer;
    typedef int gint;
    typedef gint gboolean;
    typedef char gchar;
}

#endif
