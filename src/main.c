/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zephyr/types.h>
#include <stdbool.h>
#include <data/json.h>

struct car
{
	const char *name;
	int passengers;
};

static const struct json_obj_descr car_descriptor[] = {
	JSON_OBJ_DESCR_PRIM(struct car, name, JSON_TOK_STRING),
	JSON_OBJ_DESCR_PRIM(struct car, passengers, JSON_TOK_NUMBER)
};


/*!
	\brief Decode (deserialize) JSON char to struct.
*/
struct car decode(char *encoded)
{
	struct car response;
	int ret;

	ret = json_obj_parse(
		encoded,
		sizeof(encoded) - 1,
		car_descriptor,
		ARRAY_SIZE(car_descriptor),
		&response);

	printk("\tDecode Return Value: %d\n", ret);

	// TODO: error checking

	return response;
}

/*!
	\brief Encode (Serialize) response to JSON char.
*/
char * encode(struct car response)
{
	int ret;
	ssize_t len;

	len = json_calc_encoded_len(car_descriptor, ARRAY_SIZE(car_descriptor), &response);

	char buffer[len];

	ret = json_obj_encode_buf(
		car_descriptor,
		ARRAY_SIZE(car_descriptor),
		&response,
		buffer, 
		sizeof(buffer));

	printk("\tEncode Return Value: %d\n", ret);

	// TODO: handle error
	
	return buffer;
}

void main(void)
{
	printk("Hello JSON! %s\n\n", CONFIG_BOARD);
	
	char *json = "{\"car\": \"Ford\",\"passengers\": 5}";
	printk("JSON: %s\n", json);

	printk("Starting to decode json string to struct.\n");
	struct car response = decode(json);

	char *value = "Ford";
	if(!strcmp(value, response.name))
		printk("\tCar name is correct.\n");
	else
		printk("\tCar name is NOT correct.\n");

	printk("\tCar name compare result: %d\n", strcmp(value, response.name));



	printk("\n\n\nStarting to encode struct to json string.\n");
	struct car ford = {
		.name = "Ford",
		.passengers = 5
	};

	char *encodedJson = encode(ford);

	printk("Goodbye\n");
}
