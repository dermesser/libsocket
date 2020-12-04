#!/bin/bash

gcc -lsocket -o srv unix_stream_server.c
gcc -lsocket -o cl  unix_stream_client.c

