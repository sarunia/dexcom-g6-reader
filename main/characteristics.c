#include "dexcom_g6_reader.h"

const char* tag_chrs = "[Dexcom-G6-Reader][chrs]";

void
dgr_add_to_list(list *l, const struct ble_gatt_chr *in) {
    list_element *le = (list_element*) malloc(sizeof(list_element));
    le->data = *in;
    le->next = NULL;

    if(l->length == 0) {
        l->head = le;
        l->tail = le;
        l->length++;
    } else {
        l->tail->next = le;
        l->length++;
        l->tail = le;
    }
}

int
dgr_find_in_list(list *l, const ble_uuid_t *uuid, struct ble_gatt_chr *out) {
    list_element le = *l->head;

    while(true) {
        if(ble_uuid_cmp(uuid, &le.data.uuid.u) == 0) {
            *out = le.data;
            return 0;
        } else if(le.next != NULL) {
            le = *le.next;
        } else {
            char buf[BLE_UUID_STR_LEN];
            ble_uuid_to_str(uuid, buf);
            ESP_LOGE(tag_chrs, "Could not find uuid = %s in characteristics list.", buf);
            return -1;
        }
    }
}

void
dgr_print_list(list *l) {
    char buf[BLE_UUID_STR_LEN];
    list_element le = *l->head;

    ESP_LOGI(tag_chrs, "List of characteristics:");
    while(true) {
        ble_uuid_to_str(&le.data.uuid.u, buf);
        ESP_LOGI(tag_chrs, "\tuuid = %s", buf);
        ESP_LOGI(tag_chrs, "\t\tval_handle = %d", le.data.val_handle);
        ESP_LOGI(tag_chrs, "\t\tdef_handle = %d", le.data.def_handle);
        ESP_LOGI(tag_chrs, "\t\tproperties = %d", le.data.properties);

        if(le.next == NULL) {
            break;
        } else {
            le = *le.next;
        }
    }
}