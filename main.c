/*
4chan /dpt/ bump limit detector

Written in 2021 by HowardHateart email: howardhateart[at] yandex [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curl/curl.h>

#define HLH_JSON_IMPLEMENTATION
#include "external/HLH_json.h"
//-------------------------------------

//Internal includes
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
typedef struct
{
   char *memory;
   size_t size;
}Memory;
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
static size_t write_memory(void *contents, size_t size, size_t nmemb, void *userp);
//-------------------------------------

//Function implementations

int main(int argc, char **argv)
{
   CURL *curl_handle;
   CURLcode res;

   Memory chunk;
   chunk.memory = malloc(1);
   chunk.size = 0;

   curl_global_init(CURL_GLOBAL_ALL);
   curl_handle = curl_easy_init();
   curl_easy_setopt(curl_handle,CURLOPT_URL,"https://a.4cdn.org/g/catalog.json");
   curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory);

   curl_easy_setopt(curl_handle,CURLOPT_WRITEDATA,(void *)&chunk);
   curl_easy_setopt(curl_handle,CURLOPT_USERAGENT,"libcurl-agent/1.0");

   res = curl_easy_perform(curl_handle);

   if(res!=CURLE_OK)
   {
      printf("Failed to get file: %s\n",curl_easy_strerror(res));
   }
   else
   {
      puts(chunk.memory);
   }

   curl_easy_cleanup(curl_handle);
   free(chunk.memory);
   curl_global_cleanup();
 
   return 0;
}

static size_t write_memory(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  Memory *mem = (Memory *)userp;
 
  char *ptr = realloc(mem->memory,mem->size+realsize + 1);

  //TODO: check if failed
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
//-------------------------------------
