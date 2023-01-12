#include <stdio.h>
#include <stdlib.h>

#include <mosquitto.h>

void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	printf("ID: %d\n", *(int *)obj);
	if (rc)
	{
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "announce", 0);
	mosquitto_subscribe(mosq, NULL, "onoff", 0);
	mosquitto_subscribe(mosq, NULL, "battery", 0);
	mosquitto_subscribe(mosq, NULL, "alive", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	printf("New message with topic %s: %s\n", msg->topic, (char *)msg->payload);
}

int main()
{
	int rc, id = 12;

	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new("subscribe-test", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	rc = mosquitto_connect(mosq, "localhost", 1883, 10);
	if (rc)
	{
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);

	printf('D1');
	printf('D2');
	printf("D3");
	printf("F1");
	printf("F2");
	printf("D5");
	printf("D6");
	printf("F3");
	printf("F4");

	while(1);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
