#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "wifi_hal_priv.h"
#include "wifi_hal.h"

#define NULL_CHAR '\0'
#define NEW_LINE '\n'
#define MAX_BUF_SIZE 128
#define MAX_CMD_SIZE 1024
#define RPI_LEN_32 32
#define INVALID_KEY                      "12345678"

int wifi_nvram_defaultRead(char *in,char *out);
int _syscmd(char *cmd, char *retBuf, int retBufSize);


/* FIXME: VIKAS/PRAMOD:
 * If wifi_nvram_defaultRead fail, handle appropriately in callers.
 */
int wifi_nvram_defaultRead(char *in,char *out)
{
    char buf[MAX_BUF_SIZE]={'\0'};
    char cmd[MAX_CMD_SIZE]={'\0'};
    char *position;

    sprintf(cmd,"grep '%s=' /nvram/wifi_defaults.txt",in);
    if(_syscmd(cmd,buf,sizeof(buf)) == -1)
    {
        wifi_hal_dbg_print("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
        return -1;
    }

    if (buf[0] == NULL_CHAR)
        return -1;
    position = buf;
    while(*position != NULL_CHAR)
    {
        if (*position == NEW_LINE)
        {
            *position = NULL_CHAR;
            break;
        }
        position++;
    }
    position = strchr(buf, '=');
    if (position == NULL)
    {
        wifi_hal_dbg_print("Line %d: invalid line '%s'",__LINE__, buf);
        return -1;
    }
    *position = NULL_CHAR;
    position++;
    strncpy(out,position,strlen(position)+1);
    return 0; 
}

int platform_pre_init()
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_post_init(wifi_vap_info_map_t *vap_map)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    system("brctl addif brlan0 wlan0");
    system("brctl addif brlan0 wlan1");
    return 0;
}


int platform_set_radio(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_set_radio_pre_init(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_create_vap(wifi_radio_index_t index, wifi_vap_info_map_t *map)
{
    char output_val[RPI_LEN_32];
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);

    if (map == NULL)
    {
        wifi_hal_dbg_print("%s:%d: wifi_vap_info_map_t *map is NULL \n", __func__, __LINE__);
    }
    for (index = 0; index < map->num_vaps; index++)
    {
      if (map->vap_array[index].vap_mode == wifi_vap_mode_ap)
      {
	//   Assigning default radius values 
	    wifi_nvram_defaultRead("radius_s_port",output_val);
	    map->vap_array[index].u.bss_info.security.u.radius.s_port = atoi(output_val);
	    wifi_nvram_defaultRead("radius_s_ip",map->vap_array[index].u.bss_info.security.u.radius.s_ip);
	    wifi_nvram_defaultRead("radius_key",map->vap_array[index].u.bss_info.security.u.radius.s_key);
      }
    } 
    return 0;
}

int nvram_get_radio_enable_status(bool *radio_enable, int radio_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int nvram_get_vap_enable_status(bool *vap_enable, int vap_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int nvram_get_current_security_mode(wifi_security_modes_t *security_mode,int vap_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_get_keypassphrase_default(char *password, int vap_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);  
    /*password is not sensitive,won't grant access to real devices*/ 
    wifi_nvram_defaultRead("rpi_wifi_password",password);
    if (strlen(password) == 0) {
       wifi_hal_error_print("%s:%d nvram default password not found, "
           "enforced alternative default password\n", __func__, __LINE__);
       strncpy(password, INVALID_KEY, strlen(INVALID_KEY) + 1);
    }
    return 0;
}

int platform_get_ssid_default(char *ssid, int vap_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);   
    sprintf(ssid,"RPI_RDKB-AP%d",vap_index);
    return 0;
}

int platform_get_wps_pin_default(char *pin)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);  
    wifi_nvram_defaultRead("wps_pin",pin);
    return 0;
}

int platform_wps_event(wifi_wps_event_t data)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);  
    return 0;
}

int platform_get_country_code_default(char *code)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);  
    strcpy(code,"US");
    return 0;
}

int nvram_get_current_password(char *l_password, int vap_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);
    /*password is not sensitive,won't grant access to real devices*/ 
    wifi_nvram_defaultRead("rpi_wifi_password",l_password);
    return 0;
}

int nvram_get_current_ssid(char *l_ssid, int vap_index)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__); 
    sprintf(l_ssid,"RPI_RDKB-AP%d",vap_index);
    return 0;
}

int platform_pre_create_vap(wifi_radio_index_t index, wifi_vap_info_map_t *map)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_flags_init(int *flags)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);
    *flags = PLATFORM_FLAGS_STA_INACTIVITY_TIMER;
    return 0;
}

int platform_get_aid(void* priv, u16* aid, const u8* addr)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_free_aid(void* priv, u16* aid)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_sync_done(void* priv)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_get_channel_bandwidth(wifi_radio_index_t index,  wifi_channelBandwidth_t *channelWidth)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_update_radio_presence(void)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int nvram_get_mgmt_frame_power_control(int vap_index, int* output_dbm)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);    
    return 0;
}

int platform_set_txpower(void* priv, uint txpower)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);
    return 0;
}

int platform_set_offload_mode(void* priv, uint offload_mode)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);
    return RETURN_OK;
}

int platform_get_radius_key_default(char *radius_key)
{
    wifi_hal_dbg_print("%s:%d \n",__func__,__LINE__);
    wifi_nvram_defaultRead("radius_key",radius_key);
    return 0;	
}

int platform_get_acl_num(int vap_index, uint *acl_count)
{
    return 0;
}

int platform_get_vendor_oui(char *vendor_oui, int vendor_oui_len)
{
    return -1;
}

int platform_set_neighbor_report(uint index, uint add, mac_address_t mac)
{
    return 0;
}

int platform_get_radio_phytemperature(wifi_radio_index_t index,
    wifi_radioTemperature_t *radioPhyTemperature)
{
    return 0;
}

int platform_set_dfs(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
    return 0;
}

int wifi_startNeighborScan(INT apIndex, wifi_neighborScanMode_t scan_mode, INT dwell_time, UINT chan_num, UINT *chan_list)
{
    return wifi_hal_startNeighborScan(apIndex, scan_mode, dwell_time, chan_num, chan_list);
}

int wifi_getNeighboringWiFiStatus(INT radio_index, wifi_neighbor_ap2_t **neighbor_ap_array, UINT *output_array_size)
{
    return wifi_hal_getNeighboringWiFiStatus(radio_index, neighbor_ap_array, output_array_size);
}

int wifi_setQamPlus(void *priv)
{
    return 0;
}

int wifi_setApRetrylimit(void *priv)
{
    return 0;
}


INT wifi_getRadioChannelStats(INT radioIndex, wifi_channelStats_t *input_output_channelStats_array,
    INT array_size)
{
    return RETURN_OK;
}
//--------------------------------------------------------------------------------------------------
INT wifi_getApEnable(INT apIndex, BOOL *output_bool)
{
    return RETURN_OK;
}

//--------------------------------------------------------------------------------------------------
INT wifi_setApMacAddressControlMode(INT apIndex, INT filterMode)
{
    return RETURN_OK;
}


//--------------------------------------------------------------------------------------------------
INT wifi_getBssLoad(INT apIndex, BOOL *enabled)
{
    return RETURN_ERR;
}

//--------------------------------------------------------------------------------------------------
INT wifi_setLayer2TrafficInspectionFiltering(INT apIndex, BOOL enabled)
{
    return RETURN_ERR;
}
//--------------------------------------------------------------------------------------------------
INT wifi_setApIsolationEnable(INT apIndex, BOOL enable)
{
    return RETURN_ERR;
}

int platform_get_radio_caps(wifi_radio_index_t index)
{ 
    return 0;
}

INT wifi_getApDeviceRSSI(INT ap_index, CHAR *MAC, INT *output_RSSI)
{
    return 0;
}

INT wifi_getApAssociatedDeviceDiagnosticResult3(INT apIndex,
    wifi_associated_dev3_t **associated_dev_array, UINT *output_array_size)
{
    return 0;
}

INT wifi_setRadioDfsAtBootUpEnable(INT radioIndex, BOOL enable) // Tr181
{
    return 0;
}

INT wifi_getRadioChannel(INT radioIndex, ULONG *output_ulong)
{
    return 0;
}

INT wifi_steering_eventRegister(wifi_steering_eventCB_t event_cb)
{
    return RETURN_OK;
}

int wifi_rrm_send_beacon_req(struct wifi_interface_info_t *interface, const u8 *addr,
    u16 num_of_repetitions, u8 measurement_request_mode, u8 oper_class, u8 channel,
    u16 random_interval, u16 measurement_duration, u8 mode, const u8 *bssid,
    struct wpa_ssid_value *ssid, u8 *rep_cond, u8 *rep_cond_threshold, u8 *rep_detail,
    const u8 *ap_ch_rep, unsigned int ap_ch_rep_len, const u8 *req_elem, unsigned int req_elem_len,
    u8 *ch_width, u8 *ch_center_freq0, u8 *ch_center_freq1, u8 last_indication)
{
    return 0;
}

/* called by BTM API */
int wifi_wnm_send_bss_tm_req(struct wifi_interface_info_t *interface, struct sta_info *sta,
    u8 dialog_token, u8 req_mode, int disassoc_timer, u8 valid_int, const u8 *bss_term_dur,
    const char *url, const u8 *nei_rep, size_t nei_rep_len, const u8 *mbo_attrs, size_t mbo_len)
{
    return 0;
}

int handle_wnm_action_frame(struct wifi_interface_info_t *interface, const mac_address_t sta,
    struct ieee80211_mgmt *mgmt, size_t len)
{
    return 0;
}

int handle_rrm_action_frame(struct wifi_interface_info_t *interface, const mac_address_t sta,
    const struct ieee80211_mgmt *mgmt, size_t len, int ssi_signal)
{
    return 0;
}

INT wifi_setApManagementFramePowerControl(INT apIndex, INT dBm)
{
    return 0;
}

int update_hostap_mlo(wifi_interface_info_t *interface)
{
    return 0;
}

int wifi_drv_set_ap_mlo(struct nl_msg *msg, void *priv, struct wpa_driver_ap_params *params)
{
    return 0;
}

void wifi_drv_get_phy_eht_cap_mac(struct eht_capabilities *eht_capab, struct nlattr **tb)
{
}

INT wifi_steering_clientDisconnect(UINT steeringgroupIndex, INT apIndex, mac_address_t client_mac,
    wifi_disconnectType_t type, UINT reason)
{
    return 0;
}

INT wifi_setProxyArp(INT apIndex, BOOL enabled)
{
    return 0;
}

INT wifi_setCountryIe(INT apIndex, BOOL enabled)
{
    return 0;
}

INT wifi_getLayer2TrafficInspectionFiltering(INT apIndex, BOOL *enabled)
{
    return 0;
}

INT wifi_getCountryIe(INT apIndex, BOOL *enabled)
{
    return 0;
}

INT wifi_setP2PCrossConnect(INT apIndex, BOOL disabled)
{
    return 0;
}

INT wifi_getDownStreamGroupAddress(INT apIndex, BOOL *disabled)
{
    return 0;
}

INT wifi_getProxyArp(INT apIndex, BOOL *enabled)
{
    return 0;
}

INT wifi_applyGASConfiguration(wifi_GASConfiguration_t *input_struct)
{
    return 0;
}

INT wifi_pushApHotspotElement(INT apIndex, BOOL enabled)
{
    return 0;
}

INT wifi_setBssLoad(INT apIndex, BOOL enabled)
{
    return 0;
}

INT wifi_getApInterworkingServiceEnable(INT apIndex, BOOL *output_bool)
{
    return 0;
}

INT wifi_sendActionFrame(INT apIndex, mac_address_t MacAddr, UINT frequency, UCHAR *frame, UINT len)
{
    return 0;
}

INT wifi_setDownStreamGroupAddress(INT apIndex, BOOL disabled)
{
    return 0;
}
INT wifi_getApAssociatedClientDiagnosticResult(INT ap_index, char *key,wifi_associated_dev3_t *assoc)
{
    return RETURN_ERR;
}