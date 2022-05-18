#include "mqtttasks.h"

void *client_refresher(void *client)
{
    while (1)
    {
        mqtt_sync((struct mqtt_client *)client);
        usleep(100000U);
    }
    return NULL;
}

void publish_callback(void **unused, struct mqtt_response_publish *published)
{
}

void subscribe_callback(void **unused, struct mqtt_response_publish *suback)
{
    /* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
    char *topic_name = (char *)malloc(suback->topic_name_size + 1);
    memcpy(topic_name, suback->topic_name, suback->topic_name_size);
    topic_name[suback->topic_name_size] = '\0';

    printf("Received publish('%s'): %s\n", topic_name, (const char *)suback->application_message);

    free(topic_name);
}

int pub()
{
    struct mqtt_client client;
    /* open the non-blocking TCP socket (connecting to the broker) */
    const char *addr;
    const char *port;
    addr = "localhost";
    port = "1883";
    int sockfd = open_nb_socket(addr, port);
    /* instantiate the client */
    uint8_t sendbuf[2048];                                                                            /* sendbuf should be large enough to hold multiple whole mqtt messages */
    uint8_t recvbuf[1024];                                                                            /* recvbuf should be large enough any whole mqtt message expected to be received */
    mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback); /* initialize the client */

    /* Create an anonymous session */
    const char *client_id = NULL;
    /* Ensure we have a clean session */
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400); /* send a connection request to the broker. */

    /* subscribe to "toaster/temperature" with a max QoS level of 0 */
    // mqtt_subscribe(&client, "temperature", 0);
    pthread_t client_daemon;
    if (pthread_create(&client_daemon, NULL, client_refresher, &client))
    {
        fprintf(stderr, "Failed to start client daemon.\n");
    }

    // /* publish coffee temperature with a QoS level of 1 */
    for (int i = 0; i < 10; i++)
    {
        // char buf[100];
        char *buf = (char *)malloc(100);
        sprintf(buf, "temperature: %d", i);
        mqtt_publish(&client, "datetime", buf, strlen(buf) + 1, 1);
        free(buf);
        sleep(1);
    }

    return 0;
}

int sub()
{
    struct mqtt_client client;
    /* open the non-blocking TCP socket (connecting to the broker) */
    const char *addr;
    const char *port;
    addr = "localhost";
    port = "1883";
    int sockfd = open_nb_socket(addr, port);

    if (sockfd == -1)
    {
        perror("Failed to open socket: ");
    }
    /* instantiate the client */
    uint8_t sendbuf[2048];                                                                              /* sendbuf should be large enough to hold multiple whole mqtt messages */
    uint8_t recvbuf[1024];                                                                              /* recvbuf should be large enough any whole mqtt message expected to be received */
    mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), subscribe_callback); /* initialize the client */

    /* Create an anonymous session */
    const char *client_id = NULL;
    /* Ensure we have a clean session */
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400); /* send a connection request to the broker. */
    if (client.error != MQTT_OK)
    {
        fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
    }

    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    pthread_t client_daemon;
    if (pthread_create(&client_daemon, NULL, client_refresher, &client))
    {
        fprintf(stderr, "Failed to start client daemon.\n");
    }
    /* subscribe to "toaster/temperature" with a max QoS level of 0 */
    mqtt_subscribe(&client, "datetime", 1);

    // getchar();
    sleep(10);

    return 0;
}