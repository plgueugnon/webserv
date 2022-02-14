#!/bin/bash

./webserv ./etc/missing_args.conf
./webserv ./etc/http_missing.conf

./webserv ./etc/autoindex_semicolon.conf
./webserv ./etc/autoindex_wrong.conf
./webserv ./etc/autoindex_missing.conf

./webserv ./etc/index_semicolon.conf
./webserv ./etc/index_wrong.conf
./webserv ./etc/index_missing.conf

./webserv ./etc/root_semicolon.conf
./webserv ./etc/root_missing.conf

./webserv ./etc/body_size_missing.conf
./webserv ./etc/body_size_neg.conf
./webserv ./etc/body_size_digit.conf

./webserv ./etc/listen_digit.conf
./webserv ./etc/listen_port_range.conf
./webserv ./etc/listen_semicolon.conf
./webserv ./etc/listen_missing.conf

./webserv ./etc/server_bracket.conf

./webserv ./etc/server_name_missing.conf
./webserv ./etc/server_name_semicolon.conf

./webserv ./etc/location_bracket.conf
./webserv ./etc/location_slash.conf

./webserv ./etc/error_page_semicolon.conf
./webserv ./etc/error_page_code.conf
./webserv ./etc/error_page_uri.conf

./webserv ./etc/except_method.conf
./webserv ./etc/except_semicolon.conf

./webserv ./etc/return_semicolon.conf
./webserv ./etc/return_uri.conf