#include "utils.h"
#include "globals.h"

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

int json_conv(char* JSON_STRING) {
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
                   sizeof(t) / sizeof(t[0]));
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        unsigned int length = t[i + 1].end - t[i + 1].start;
        char val[length + 1];
        char g[strlen(val)+1];
        char p[strlen(val)+1];
        memcpy(val, &JSON_STRING[t[i+1].start], length);
        val[length] = '\0';

        if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
            printf("Type: %s\n", val);
            i++;
        } else if (jsoneq(JSON_STRING, &t[i], "group_id") == 0) {
            /* We may additionally check if the value is either "true" or "false" */
            printf("Group: %s\n", val);

            memcpy(g, val , strlen(val));
            g[strlen(val)] = '\0';
            group_id = g;

            printf("Group_var: %s\n", group_id);

            i++;
        } else if (jsoneq(JSON_STRING, &t[i], "player_id") == 0) {
            /* We may want to do strtol() here to get numeric value */
            printf("Player: %s\n", val);

            memcpy(p, val , strlen(val));
            p[strlen(val)] = '\0';
            player_id = p;

            printf("Player_var: %s\n", player_id);
            i++;
        } else if (jsoneq(JSON_STRING, &t[i], "group_token") == 0) {
            /* We may want to do strtol() here to get numeric value */
            printf("Token: %s\n", val);
            i++;
        } else if (jsoneq(JSON_STRING, &t[i], "status") == 0) {
            /* We may want to do strtol() here to get numeric value */
            printf("gr: %s", group_id);
            printf("Status: %s\n", val);

            i++;
        }
    }
    return EXIT_SUCCESS;
}

