#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <urweb.h>

typedef struct {
  uw_context ctx;
  char *filename;
  size_t size;
  unsigned char *data;
} save_callback_data;

void save_callback_commit(void *data) {
  save_callback_data *save_data;
  FILE *fptr;

  save_data = data;
  
  fptr = fopen(save_data->filename, "w+");
  if(!fptr) {
    uw_set_error_message(save_data->ctx, "fopen failed");
    return;
  }
  
  if(fwrite(save_data->data, save_data->size, 1, fptr) != 1) {
    uw_set_error_message(save_data->ctx, "writing failed");
    return;
  }
  fclose(fptr);
}

void save_callback_rollback(void *data) {
  save_callback_data *save_data;

  save_data = data;
  
  unlink(save_data->filename);
}

void save_callback_free(void *data, int will_retry) {
  save_callback_data *save_data;

  save_data = data;
  
  free(save_data->data);
  free(save_data->filename);
  free(save_data);
}

uw_Basis_bool uw_File_save(uw_context ctx, uw_Basis_string s, uw_Basis_file u) {
  save_callback_data *save_data;

  save_data = malloc(sizeof(save_callback_data));
  if(!save_data) {
    return 0;
  }
  save_data->ctx = ctx;

  save_data->filename = malloc(strlen(s) + 1);
  if(!save_data->filename) {
    free(save_data);
    return 0;
  }
  strcpy(save_data->filename, s);
  save_data->size = u.data.size;
  save_data->data = malloc(u.data.size);
  if(!save_data->data) {
    free(save_data->filename);
    free(save_data);
    return 0;
  }
  memcpy(save_data->data, u.data.data, u.data.size);
  
  if(!uw_register_transactional(ctx, save_data, save_callback_commit, save_callback_rollback, save_callback_free)) {
    return 1;
  }
  else {
    free(save_data->data);
    free(save_data->filename);
    free(save_data);
    return 0;
  }
}
