/*
 * Mana QML plugin
 * Copyright (C) 2013  Thorbjørn Lindeijer
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "messagenames.h"

#include <QHash>

namespace Mana {

//
// List of message names mapped by id. Generated using the following regular
// expression on the protocol header file:
//
//  Search:  ([A-Z_]+) *= (0x[0-f]+),
//  Replace: hash.insert(\2, "\1");
//
// Would be nice to auto-generate this list (for example based on macros),
// but this will do for now.
//

static QHash<unsigned short, const char*> messages()
{
    QHash<unsigned short, const char*> hash;

    hash.insert(0x0000, "PAMSG_REGISTER");
    hash.insert(0x0002, "APMSG_REGISTER_RESPONSE");
    hash.insert(0x0003, "PAMSG_UNREGISTER");
    hash.insert(0x0004, "APMSG_UNREGISTER_RESPONSE");
    hash.insert(0x0005, "PAMSG_REQUEST_REGISTER_INFO");
    hash.insert(0x0006, "APMSG_REGISTER_INFO_RESPONSE");
    hash.insert(0x0010, "PAMSG_LOGIN");
    hash.insert(0x0012, "APMSG_LOGIN_RESPONSE");
    hash.insert(0x0013, "PAMSG_LOGOUT");
    hash.insert(0x0014, "APMSG_LOGOUT_RESPONSE");
    hash.insert(0x0015, "PAMSG_LOGIN_RNDTRGR");
    hash.insert(0x0016, "APMSG_LOGIN_RNDTRGR_RESPONSE");
    hash.insert(0x0020, "PAMSG_CHAR_CREATE");
    hash.insert(0x0021, "APMSG_CHAR_CREATE_RESPONSE");
    hash.insert(0x0022, "PAMSG_CHAR_DELETE");
    hash.insert(0x0023, "APMSG_CHAR_DELETE_RESPONSE");
    hash.insert(0x0024, "APMSG_CHAR_INFO");
    hash.insert(0x0026, "PAMSG_CHAR_SELECT");
    hash.insert(0x0027, "APMSG_CHAR_SELECT_RESPONSE");
    hash.insert(0x0030, "PAMSG_EMAIL_CHANGE");
    hash.insert(0x0031, "APMSG_EMAIL_CHANGE_RESPONSE");
    hash.insert(0x0034, "PAMSG_PASSWORD_CHANGE");
    hash.insert(0x0035, "APMSG_PASSWORD_CHANGE_RESPONSE");
    hash.insert(0x0050, "PGMSG_CONNECT");
    hash.insert(0x0051, "GPMSG_CONNECT_RESPONSE");
    hash.insert(0x0053, "PCMSG_CONNECT");
    hash.insert(0x0054, "CPMSG_CONNECT_RESPONSE");
    hash.insert(0x0060, "PGMSG_DISCONNECT");
    hash.insert(0x0061, "GPMSG_DISCONNECT_RESPONSE");
    hash.insert(0x0063, "PCMSG_DISCONNECT");
    hash.insert(0x0064, "CPMSG_DISCONNECT_RESPONSE");
    hash.insert(0x0065, "PAMSG_RECONNECT");
    hash.insert(0x0066, "APMSG_RECONNECT_RESPONSE");
    hash.insert(0x0100, "GPMSG_PLAYER_MAP_CHANGE");
    hash.insert(0x0101, "GPMSG_PLAYER_SERVER_CHANGE");
    hash.insert(0x0110, "PGMSG_PICKUP");
    hash.insert(0x0111, "PGMSG_DROP");
    hash.insert(0x0112, "PGMSG_EQUIP");
    hash.insert(0x0113, "PGMSG_UNEQUIP");
    hash.insert(0x0120, "GPMSG_INVENTORY");
    hash.insert(0x0121, "GPMSG_INVENTORY_FULL");
    hash.insert(0x0122, "GPMSG_EQUIP");
    hash.insert(0x0123, "GPMSG_EQUIP_RESPONSE");
    hash.insert(0x0124, "GPMSG_UNEQUIP");
    hash.insert(0x0125, "GPMSG_UNEQUIP_RESPONE");
    hash.insert(0x0130, "GPMSG_PLAYER_ATTRIBUTE_CHANGE");
    hash.insert(0x0140, "GPMSG_ATTRIBUTE_POINTS_STATUS");
    hash.insert(0x0160, "PGMSG_RAISE_ATTRIBUTE");
    hash.insert(0x0161, "GPMSG_RAISE_ATTRIBUTE_RESPONSE");
    hash.insert(0x0170, "PGMSG_LOWER_ATTRIBUTE");
    hash.insert(0x0171, "GPMSG_LOWER_ATTRIBUTE_RESPONSE");
    hash.insert(0x0180, "PGMSG_RESPAWN");
    hash.insert(0x0200, "GPMSG_BEING_ENTER");
    hash.insert(0x0201, "GPMSG_BEING_LEAVE");
    hash.insert(0x0202, "GPMSG_ITEM_APPEAR");
    hash.insert(0x0210, "GPMSG_BEING_LOOKS_CHANGE");
    hash.insert(0x0211, "GPMSG_BEING_EMOTE");
    hash.insert(0x0212, "PGMSG_BEING_EMOTE");
    hash.insert(0x0260, "PGMSG_WALK");
    hash.insert(0x0270, "PGMSG_ACTION_CHANGE");
    hash.insert(0x0271, "GPMSG_BEING_ACTION_CHANGE");
    hash.insert(0x0272, "PGMSG_DIRECTION_CHANGE");
    hash.insert(0x0273, "GPMSG_BEING_DIR_CHANGE");
    hash.insert(0x0274, "GPMSG_BEING_HEALTH_CHANGE");
    hash.insert(0x0280, "GPMSG_BEINGS_MOVE");
    hash.insert(0x0281, "GPMSG_ITEMS");
    hash.insert(0x0282, "GPMSG_BEING_ABILITY_POINT");
    hash.insert(0x0283, "GPMSG_BEING_ABILITY_BEING");
    hash.insert(0x0292, "PGMSG_USE_ABILITY_ON_BEING");
    hash.insert(0x0293, "GPMSG_ABILITY_STATUS");
    hash.insert(0x0294, "PGMSG_USE_ABILITY_ON_POINT");
    hash.insert(0x0295, "GPMSG_ABILITY_REMOVED");
    hash.insert(0x0296, "GPMSG_ABILITY_COOLDOWN");
    hash.insert(0x02A0, "PGMSG_SAY");
    hash.insert(0x02A1, "GPMSG_SAY");
    hash.insert(0x02B0, "GPMSG_NPC_CHOICE");
    hash.insert(0x02B1, "GPMSG_NPC_MESSAGE");
    hash.insert(0x02B2, "PGMSG_NPC_TALK");
    hash.insert(0x02B3, "PGMSG_NPC_TALK_NEXT");
    hash.insert(0x02B4, "PGMSG_NPC_SELECT");
    hash.insert(0x02B5, "GPMSG_NPC_BUY");
    hash.insert(0x02B6, "GPMSG_NPC_SELL");
    hash.insert(0x02B7, "PGMSG_NPC_BUYSELL");
    hash.insert(0x02B8, "GPMSG_NPC_ERROR");
    hash.insert(0x02B9, "GPMSG_NPC_CLOSE");
    hash.insert(0x02D0, "GPMSG_NPC_POST");
    hash.insert(0x02D1, "PGMSG_NPC_POST_SEND");
    hash.insert(0x02D2, "GPMSG_NPC_POST_GET");
    hash.insert(0x02D3, "PGMSG_NPC_NUMBER");
    hash.insert(0x02D4, "PGMSG_NPC_STRING");
    hash.insert(0x02D5, "GPMSG_NPC_NUMBER");
    hash.insert(0x02D6, "GPMSG_NPC_STRING");
    hash.insert(0x02C0, "PGMSG_TRADE_REQUEST");
    hash.insert(0x02C1, "GPMSG_TRADE_REQUEST");
    hash.insert(0x02C2, "GPMSG_TRADE_START");
    hash.insert(0x02C3, "GPMSG_TRADE_COMPLETE");
    hash.insert(0x02C4, "PGMSG_TRADE_CANCEL");
    hash.insert(0x02C5, "GPMSG_TRADE_CANCEL");
    hash.insert(0x02C6, "PGMSG_TRADE_AGREED");
    hash.insert(0x02C7, "GPMSG_TRADE_AGREED");
    hash.insert(0x02C8, "PGMSG_TRADE_CONFIRM");
    hash.insert(0x02C9, "GPMSG_TRADE_CONFIRM");
    hash.insert(0x02CA, "PGMSG_TRADE_ADD_ITEM");
    hash.insert(0x02CB, "GPMSG_TRADE_ADD_ITEM");
    hash.insert(0x02CC, "PGMSG_TRADE_SET_MONEY");
    hash.insert(0x02CD, "GPMSG_TRADE_SET_MONEY");
    hash.insert(0x02CE, "GPMSG_TRADE_BOTH_CONFIRM");
    hash.insert(0x0300, "PGMSG_USE_ITEM");
    hash.insert(0x0301, "GPMSG_USE_RESPONSE");
    hash.insert(0x0310, "GPMSG_BEINGS_DAMAGE");
    hash.insert(0x0320, "GPMSG_CREATE_EFFECT_POS");
    hash.insert(0x0321, "GPMSG_CREATE_EFFECT_BEING");
    hash.insert(0x0322, "GPMSG_CREATE_TEXT_PARTICLE");
    hash.insert(0x0330, "GPMSG_SHAKE");
    hash.insert(0x0350, "PCMSG_GUILD_CREATE");
    hash.insert(0x0351, "CPMSG_GUILD_CREATE_RESPONSE");
    hash.insert(0x0352, "PCMSG_GUILD_INVITE");
    hash.insert(0x0353, "CPMSG_GUILD_INVITE_RESPONSE");
    hash.insert(0x0354, "PCMSG_GUILD_ACCEPT");
    hash.insert(0x0355, "CPMSG_GUILD_ACCEPT_RESPONSE");
    hash.insert(0x0356, "PCMSG_GUILD_GET_MEMBERS");
    hash.insert(0x0357, "CPMSG_GUILD_GET_MEMBERS_RESPONSE");
    hash.insert(0x0358, "CPMSG_GUILD_UPDATE_LIST");
    hash.insert(0x0360, "PCMSG_GUILD_QUIT");
    hash.insert(0x0361, "CPMSG_GUILD_QUIT_RESPONSE");
    hash.insert(0x0365, "PCMSG_GUILD_PROMOTE_MEMBER");
    hash.insert(0x0366, "CPMSG_GUILD_PROMOTE_MEMBER_RESPONSE");
    hash.insert(0x0370, "PCMSG_GUILD_KICK_MEMBER");
    hash.insert(0x0371, "CPMSG_GUILD_KICK_MEMBER_RESPONSE");
    hash.insert(0x0372, "CPMSG_GUILD_KICK_NOTIFICATION");
    hash.insert(0x0388, "CPMSG_GUILD_INVITED");
    hash.insert(0x0389, "CPMSG_GUILD_REJOIN");
    hash.insert(0x03A0, "PGMSG_PARTY_INVITE");
    hash.insert(0x03A1, "GPMSG_PARTY_INVITE_ERROR");
    hash.insert(0x03A2, "GCMSG_PARTY_INVITE");
    hash.insert(0x03A4, "CPMSG_PARTY_INVITED");
    hash.insert(0x03A5, "PCMSG_PARTY_INVITE_ANSWER");
    hash.insert(0x03A6, "CPMSG_PARTY_INVITE_ANSWER_RESPONSE");
    hash.insert(0x03A8, "CPMSG_PARTY_REJECTED");
    hash.insert(0x03AA, "PCMSG_PARTY_QUIT");
    hash.insert(0x03AB, "CPMSG_PARTY_QUIT_RESPONSE");
    hash.insert(0x03B0, "CPMSG_PARTY_NEW_MEMBER");
    hash.insert(0x03B1, "CPMSG_PARTY_MEMBER_LEFT");
    hash.insert(0x0401, "CPMSG_ERROR");
    hash.insert(0x0402, "CPMSG_ANNOUNCEMENT");
    hash.insert(0x0403, "CPMSG_PRIVMSG");
    hash.insert(0x0404, "CPMSG_PUBMSG");
    hash.insert(0x0410, "PCMSG_CHAT");
    hash.insert(0x0412, "PCMSG_PRIVMSG");
    hash.insert(0x0415, "PCMSG_WHO");
    hash.insert(0x0416, "CPMSG_WHO_RESPONSE");
    hash.insert(0x0430, "CPMSG_CHANNEL_EVENT");
    hash.insert(0x0440, "PCMSG_ENTER_CHANNEL");
    hash.insert(0x0441, "CPMSG_ENTER_CHANNEL_RESPONSE");
    hash.insert(0x0443, "PCMSG_QUIT_CHANNEL");
    hash.insert(0x0444, "CPMSG_QUIT_CHANNEL_RESPONSE");
    hash.insert(0x0445, "PCMSG_LIST_CHANNELS");
    hash.insert(0x0446, "CPMSG_LIST_CHANNELS_RESPONSE");
    hash.insert(0x0460, "PCMSG_LIST_CHANNELUSERS");
    hash.insert(0x0461, "CPMSG_LIST_CHANNELUSERS_RESPONSE");
    hash.insert(0x0462, "PCMSG_TOPIC_CHANGE");
    hash.insert(0x0465, "PCMSG_USER_MODE");
    hash.insert(0x0466, "PCMSG_KICK_USER");
    hash.insert(0x0500, "GAMSG_REGISTER");
    hash.insert(0x0501, "AGMSG_REGISTER_RESPONSE");
    hash.insert(0x0502, "AGMSG_ACTIVE_MAP");
    hash.insert(0x0510, "AGMSG_PLAYER_ENTER");
    hash.insert(0x0520, "GAMSG_PLAYER_DATA");
    hash.insert(0x0530, "GAMSG_REDIRECT");
    hash.insert(0x0531, "AGMSG_REDIRECT_RESPONSE");
    hash.insert(0x0532, "GAMSG_PLAYER_RECONNECT");
    hash.insert(0x0533, "GAMSG_PLAYER_SYNC");
    hash.insert(0x0540, "GAMSG_SET_VAR_CHR");
    hash.insert(0x0541, "GAMSG_GET_VAR_CHR");
    hash.insert(0x0542, "AGMSG_GET_VAR_CHR_RESPONSE");
    hash.insert(0x0546, "GAMSG_SET_VAR_MAP");
    hash.insert(0x0547, "GAMSG_SET_VAR_WORLD");
    hash.insert(0x0548, "AGMSG_SET_VAR_WORLD");
    hash.insert(0x0550, "GAMSG_BAN_PLAYER");
    hash.insert(0x0556, "GAMSG_CHANGE_ACCOUNT_LEVEL");
    hash.insert(0x0560, "GAMSG_STATISTICS");
    hash.insert(0x0590, "CGMSG_CHANGED_PARTY");
    hash.insert(0x05A0, "GCMSG_REQUEST_POST");
    hash.insert(0x05A1, "CGMSG_POST_RESPONSE");
    hash.insert(0x05A5, "GCMSG_STORE_POST");
    hash.insert(0x05A6, "CGMSG_STORE_POST_RESPONSE");
    hash.insert(0x0600, "GAMSG_TRANSACTION");
    hash.insert(0x0601, "GAMSG_CREATE_ITEM_ON_MAP");
    hash.insert(0x0602, "GAMSG_REMOVE_ITEM_ON_MAP");
    hash.insert(0x0603, "GAMSG_ANNOUNCE");
    hash.insert(0x7FFF, "XXMSG_INVALID");

    return hash;
}

/**
 * Returns the name of the message with the given \a id, or null when the id
 * is unknown.
 */
const char *messageName(unsigned short id)
{
    static QHash<unsigned short, const char*> hash = messages();
    return hash.value(id);
}

} // namespace Mana
