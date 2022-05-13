#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <mqtt.h>
#include "posix_sockets.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void *client_refresher(void *client);
    void publish_callback(void **unused, struct mqtt_response_publish *published);
    void subscribe_callback(void **unused, struct mqtt_response_publish *suback);
    int pub();
    int sub();

#ifdef __cplusplus
}
#endif