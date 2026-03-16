@echo off

cls
echo ---------------BUILDING CLIENT---------------
call .build main_client.cpp client "-INetwork -IEngine -I. -lws2_32"

echo ---------------BUILDING SERVER---------------
call .build main_server.cpp server "-INetwork -IEngine -I. -lws2_32"
