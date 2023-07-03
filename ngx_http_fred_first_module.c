/**
 * My First Test Nginx Module
 */
 
// Import required libraries
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#define HELLO_WORLD "im fred\r\n"

static char* ngx_http_fred_first(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

//static void* ngx_http_fred_first_create_loc_conf(ngx_conf_t *cf);

//static char* ngx_http_fred_first_merge_loc_conf(ngx_conf_t *cf,
 //   void *parent, void *child);
    
static ngx_int_t ngx_http_fred_first_handler(ngx_http_request_t *r);

// This module's location configuration struct
typedef struct {
    ngx_uint_t mode;
    
    ngx_flag_t enable;
} ngx_http_fred_first_loc_conf_t;

// Module's directives in a static array of ngx_command_t
static ngx_command_t ngx_http_fred_first_commands[] = {
    { ngx_string("fred_mode"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1, // Takes 2 arguments in location context
      ngx_conf_set_num_slot, // configuration setup function called when the directive is seen
      NGX_HTTP_LOC_CONF_OFFSET, // saved to the module's location configuration
      offsetof(ngx_http_fred_first_loc_conf_t, mode), // which part of this configuration struct to write to
      NULL }, 
      
      { ngx_string("fred"), /* directive */
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, /* location context and takes
                                            no arguments*/
      ngx_http_fred_first, /* configuration setup function */
      0, /* No offset. Only one context is supported. */
      0, /* No offset when storing the module configuration on struct. */
      NULL},

    ngx_null_command // terminate the array as with this as last element
};

static u_char ngx_hello_world[] = HELLO_WORLD;

// Context for function reference for making the configurations
static ngx_http_module_t ngx_http_fred_first_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    NULL, // ngx_http_fred_first_create_loc_conf,  /* create location configuration */
    NULL, // ngx_http_fred_first_merge_loc_conf /* merge location configuration */
};

/*
// create location configuration
static void* ngx_http_fred_first_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_fred_first_loc_conf_t *conf; // pointer to the location config struct
    
    // allocate memory for the struct in the cf pool
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_fred_first_loc_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR; // Check for allocation error
    }
    conf->mode = NGX_CONF_UNSET_UINT; // tells the merging function that this value should be overriden
    return conf;
}

// merge location configuration
static char *
ngx_http_fred_first_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_fred_first_loc_conf_t *prev = parent;
    ngx_http_fred_first_loc_conf_t *conf = child;

    ngx_conf_merge_uint_value(conf->mode, prev->mode, 1);

    if (conf->mode < 1) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, 
            "value must be equal or more than 1");
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}
*/ 

// Module definition
ngx_module_t  ngx_http_fred_first_module = {
    NGX_MODULE_V1,
    &ngx_http_fred_first_module_ctx, /* module context */
    ngx_http_fred_first_commands,   /* module directives */
    NGX_HTTP_MODULE,               /* module type */
    NULL,                          /* init master */
    NULL,                          /* init module */
    NULL,                          /* init process */
    NULL,                          /* init thread */
    NULL,                          /* exit thread */
    NULL,                          /* exit process */
    NULL,                          /* exit master */
    NGX_MODULE_V1_PADDING
};

// Content Handler
static ngx_int_t ngx_http_fred_first_handler(ngx_http_request_t *r)
{
    // ngx_http_fred_first_loc_conf_t  *cglcf;
    // cglcf = ngx_http_get_module_loc_conf(r, ngx_http_fred_first_module);
    
    // Set the Content-Type Header
    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *) "tex/plain";
    
    // Buffer 
    ngx_buf_t *b;
    // Buffer Chain
    ngx_chain_t out;
    
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t)); // Allocate buffer in the request pool
    if (b == NULL) {
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
        "Failed to allocate response buffer.");
    return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    
    out.buf = b; // Insert buffer in the chain 
    out.next = NULL; // Only one buffer in the chain
    
    b->pos = ngx_hello_world; /* first position in memory of the data */
    b->last = ngx_hello_world + sizeof(ngx_hello_world) - 1; /* last position in memory of the data */
    b->memory = 1; /* content is in read-only memory */
    b->last_buf = 1; /* there will be no more buffers in the request */
    
    // Status code
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = sizeof(ngx_hello_world) - 1;
    ngx_http_send_header(r); // send the header
    
    return ngx_http_output_filter(r, &out); // send to filters
    
}

static char *ngx_http_fred_first(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf; // pointer to the location config of nginx http core
    ngx_http_fred_first_loc_conf_t *cglcf = conf;
    
    // Attach the handler to the location config
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_fred_first_handler;
    
    cglcf->enable = 1;
    
    return NGX_CONF_OK; // return the status
}
    
    


