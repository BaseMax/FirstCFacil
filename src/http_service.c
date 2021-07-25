#include "fio_cli.h"
#include "main.h"

FIOBJ HTTP_HEADER_X_DATA;

/* TODO: edit this function to handle HTTP data and answer Websocket requests.*/
static void on_http_request(http_s *request) {
  // Set a cookie
  http_set_cookie(request, .name = "my_cookie", .name_len = 9, .value = "data",
                  .value_len = 4);
  // Set content-type
  http_set_header(request, HTTP_HEADER_CONTENT_TYPE,
                  http_mimetype_find("txt", 3));
  // Set custom header
  http_set_header(request, HTTP_HEADER_X_DATA, fiobj_str_new("my data", 7));
  /* set a response and send it (finnish vs. destroy). */
  http_send_body(request, "Hello World!\r\n", 14);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  HTTP_HEADER_X_DATA = fiobj_str_new("X-Data", 6);

  /* listen for inncoming connections */
  if (http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
                  .on_request = on_http_request,
                  .max_body_size = fio_cli_get_i("-maxbd") * 1024 * 1024,
                  .ws_max_msg_size = fio_cli_get_i("-max-msg") * 1024,
                  .public_folder = fio_cli_get("-public"),
                  .log = fio_cli_get_bool("-log"),
                  .timeout = fio_cli_get_i("-keep-alive"),
                  .ws_timeout = fio_cli_get_i("-ping")) == -1) {
    /* listen failed ?*/
    perror("ERROR: facil couldn't initialize HTTP service (already running?)");
    exit(1);
  }
}
