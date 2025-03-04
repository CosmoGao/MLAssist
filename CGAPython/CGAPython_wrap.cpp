#include "CGAPython.h"
namespace py = pybind11;
//PYBIND11_MAKE_OPAQUE(std::vector<int>);
//写了这个 就必须def类  这个在Python调用 感觉也麻烦 就是效率高 另一种会默认转换为python格式
//PYBIND11_MAKE_OPAQUE(std::vector<cga_pet_skill_info_t>);		


#include <ctime>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>

std::string GetCurrentTimeStamp(int time_stamp_type = 0)
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_time_t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%F %T", now_tm);

	std::ostringstream ss;
	ss.fill('0');

	std::chrono::milliseconds ms;
	std::chrono::microseconds cs;
	std::chrono::nanoseconds ns;

	switch (time_stamp_type)
	{
	case 0:
		ss << buffer;
		break;
	case 1:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		ss << buffer << ":" << ms.count();
		break;
	case 2:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000;
		break;
	case 3:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
		ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000 << ":" << ns.count() % 1000;
		break;
	default:
		ss << buffer;
		break;
	}

	return ss.str();
}

void noisy_function(const std::string& msg, bool flush) {

	std::cout << GetCurrentTimeStamp() << "Test" << msg;
	if (flush) {
		std::cout << std::flush;
	}
}

//注册python模块
PYBIND11_MODULE(CGAPython, m) {
	m.doc() = "pybind11 CGAPython";
	//m.def("CreateInterface", &CGA::CreateInterface);
	add_ostream_redirect(m);
	m.def("noisy_function", &noisy_function, py::arg("msg"), py::arg("flush") = true);
	py::class_<CGA::cga_game_data_t>(m, "cga_game_data_t")
		.def(py::init<>())
		.def_readwrite("reserved", &CGA::cga_game_data_t::reserved);	
	py::class_<CGA::cga_sys_time_t>(m, "cga_sys_time_t")
		.def(py::init<>())
		.def_readwrite("years", &CGA::cga_sys_time_t::years)
		.def_readwrite("month", &CGA::cga_sys_time_t::month)
		.def_readwrite("days", &CGA::cga_sys_time_t::days)
		.def_readwrite("hours", &CGA::cga_sys_time_t::hours)
		.def_readwrite("mins", &CGA::cga_sys_time_t::mins)
		.def_readwrite("secs", &CGA::cga_sys_time_t::secs)
		.def_readwrite("local_time", &CGA::cga_sys_time_t::local_time)
		.def_readwrite("server_time", &CGA::cga_sys_time_t::server_time)
		;
	py::class_<CGA::cga_card_info_t>(m, "cga_card_info_t")
		.def(py::init<>())
		.def_readwrite("index", &CGA::cga_card_info_t::index)
		.def_readwrite("level", &CGA::cga_card_info_t::level)
		.def_readwrite("avatar", &CGA::cga_card_info_t::avatar)
		.def_readwrite("server", &CGA::cga_card_info_t::server)
		.def_readwrite("name", &CGA::cga_card_info_t::name)
		.def_readwrite("nickname", &CGA::cga_card_info_t::nickname)
		.def_readwrite("familyname", &CGA::cga_card_info_t::familyname)
		;
	py::class_<CGA::tCard_Recv_Msg>(m, "tCard_Recv_Msg")
		.def(py::init<>())
		.def_readwrite("state", &CGA::tCard_Recv_Msg::state)
		.def_readwrite("date", &CGA::tCard_Recv_Msg::date)
		.def_readwrite("msg", &CGA::tCard_Recv_Msg::msg)
		;
	py::class_<pcga_card_recv_msg_t>(m, "cga_card_recv_msg_t")
		.def(py::init<>())
		.def_readwrite("index", &pcga_card_recv_msg_t::index)
		.def_readwrite("name", &pcga_card_recv_msg_t::name)
		.def_readwrite("msgs", &pcga_card_recv_msg_t::msgs)
		;
	py::class_<CGA::cga_picbook_info_t>(m, "cga_picbook_info_t")
		.def(py::init<>())
		.def_readwrite("can_catch", &CGA::cga_picbook_info_t::can_catch)
		.def_readwrite("card_type", &CGA::cga_picbook_info_t::card_type)
		.def_readwrite("race", &CGA::cga_picbook_info_t::race)
		.def_readwrite("index", &CGA::cga_picbook_info_t::index)
		.def_readwrite("image_id", &CGA::cga_picbook_info_t::image_id)
		.def_readwrite("rate_endurance", &CGA::cga_picbook_info_t::rate_endurance)
		.def_readwrite("rate_strength", &CGA::cga_picbook_info_t::rate_strength)
		.def_readwrite("rate_defense", &CGA::cga_picbook_info_t::rate_defense)
		.def_readwrite("rate_agility", &CGA::cga_picbook_info_t::rate_agility)
		.def_readwrite("rate_magical", &CGA::cga_picbook_info_t::rate_magical)
		.def_readwrite("element_earth", &CGA::cga_picbook_info_t::element_earth)
		.def_readwrite("element_water", &CGA::cga_picbook_info_t::element_water)
		.def_readwrite("element_fire", &CGA::cga_picbook_info_t::element_fire)
		.def_readwrite("element_wind", &CGA::cga_picbook_info_t::element_wind)
		.def_readwrite("skill_slots", &CGA::cga_picbook_info_t::skill_slots)
		.def_readwrite("name", &CGA::cga_picbook_info_t::name)
		;
	py::class_<CGA::cga_game_server_info_t>(m, "cga_game_server_info_t")
		.def(py::init<>())
		.def_readwrite("ip", &cga_game_server_info_t::ip)
		.def_readwrite("port", &cga_game_server_info_t::port)
		;
	py::class_<CGA::CGAShare_t>(m, "CGAShare_t")
		.def(py::init<>())
		.def_readwrite("ProcessId", &CGA::CGAShare_t::ProcessId)
		.def_readwrite("ThreadId", &CGA::CGAShare_t::ThreadId)
		.def_readwrite("hWnd", &CGA::CGAShare_t::hWnd)
		.def_readwrite("Port", &CGA::CGAShare_t::Port)
		.def_readwrite("GameType", &CGA::CGAShare_t::GameType)
		;
	py::class_<CGA::cga_player_info_t>(m, "cga_player_info_t")
		.def(py::init<>())
		.def_readwrite("hp", &CGA::cga_player_info_t::hp)
		.def_readwrite("maxhp", &CGA::cga_player_info_t::maxhp)
		.def_readwrite("mp", &CGA::cga_player_info_t::mp)
		.def_readwrite("maxmp", &CGA::cga_player_info_t::maxmp)
		.def_readwrite("xp", &CGA::cga_player_info_t::xp)
		.def_readwrite("maxxp", &CGA::cga_player_info_t::maxxp)
		.def_readwrite("health", &CGA::cga_player_info_t::health)
		.def_readwrite("souls", &CGA::cga_player_info_t::souls)
		.def_readwrite("level", &CGA::cga_player_info_t::level)
		.def_readwrite("gold", &CGA::cga_player_info_t::gold)
		.def_readwrite("score", &CGA::cga_player_info_t::score)
		.def_readwrite("skillslots", &CGA::cga_player_info_t::skillslots)
		.def_readwrite("use_title", &CGA::cga_player_info_t::use_title)
		.def_readwrite("avatar_id", &CGA::cga_player_info_t::avatar_id)
		.def_readwrite("image_id", &CGA::cga_player_info_t::image_id)
		.def_readwrite("unitid", &CGA::cga_player_info_t::unitid)
		.def_readwrite("petid", &CGA::cga_player_info_t::petid)
		.def_readwrite("direction", &CGA::cga_player_info_t::direction)
		.def_readwrite("battle_position", &CGA::cga_player_info_t::battle_position)
		.def_readwrite("punchclock", &CGA::cga_player_info_t::punchclock)
		.def_readwrite("manu_endurance", &CGA::cga_player_info_t::manu_endurance)
		.def_readwrite("manu_skillful", &CGA::cga_player_info_t::manu_skillful)
		.def_readwrite("manu_intelligence", &CGA::cga_player_info_t::manu_intelligence)
		.def_readwrite("value_charisma", &CGA::cga_player_info_t::value_charisma)
		.def_readwrite("usingpunchclock", &CGA::cga_player_info_t::usingpunchclock)
		.def_readwrite("petriding", &CGA::cga_player_info_t::usingpunchclock)
		.def_readwrite("name", &CGA::cga_player_info_t::name)
		.def_readwrite("job", &CGA::cga_player_info_t::job)
		.def_readwrite("nick", &CGA::cga_player_info_t::nick)
		.def_readwrite("detail", &CGA::cga_player_info_t::detail)
		.def_readwrite("titles", &CGA::cga_player_info_t::titles)
		.def_readwrite("persdesc", &CGA::cga_player_info_t::persdesc)
		.def_readwrite("gid", &CGA::cga_player_info_t::gid)
		.def_readwrite("player_index", &CGA::cga_player_info_t::player_index)
		;
	py::class_<CGA::cga_pet_info_t>(m, "cga_pet_info_t")
		.def(py::init<>())
		.def_readwrite("hp", &CGA::cga_pet_info_t::hp)
		.def_readwrite("maxhp", &CGA::cga_pet_info_t::maxhp)
		.def_readwrite("mp", &CGA::cga_pet_info_t::mp)
		.def_readwrite("maxmp", &CGA::cga_pet_info_t::maxmp)
		.def_readwrite("xp", &CGA::cga_pet_info_t::xp)
		.def_readwrite("maxxp", &CGA::cga_pet_info_t::maxxp)
		.def_readwrite("health", &CGA::cga_pet_info_t::health)
		.def_readwrite("level", &CGA::cga_pet_info_t::level)
		.def_readwrite("flags", &CGA::cga_pet_info_t::flags)
		.def_readwrite("race", &CGA::cga_pet_info_t::race)
		.def_readwrite("loyality", &CGA::cga_pet_info_t::loyality)
		.def_readwrite("skillslots", &CGA::cga_pet_info_t::skillslots)
		.def_readwrite("battle_flags", &CGA::cga_pet_info_t::battle_flags)
		.def_readwrite("state", &CGA::cga_pet_info_t::state)
		.def_readwrite("index", &CGA::cga_pet_info_t::index)
		.def_readwrite("name", &CGA::cga_pet_info_t::name)
		.def_readwrite("realname", &CGA::cga_pet_info_t::realname)
		.def_readwrite("detail", &CGA::cga_pet_info_t::detail)
		;
	py::class_<CGA::cga_playerpet_detail_info_t>(m, "cga_playerpet_detail_info_t")
		.def(py::init<>())
		.def_readwrite("points_remain", &CGA::cga_playerpet_detail_info_t::points_remain)
		.def_readwrite("points_endurance", &CGA::cga_playerpet_detail_info_t::points_endurance)
		.def_readwrite("points_strength", &CGA::cga_playerpet_detail_info_t::points_strength)
		.def_readwrite("points_defense", &CGA::cga_playerpet_detail_info_t::points_defense)
		.def_readwrite("points_agility", &CGA::cga_playerpet_detail_info_t::points_agility)
		.def_readwrite("points_magical", &CGA::cga_playerpet_detail_info_t::points_magical)
		.def_readwrite("value_attack", &CGA::cga_playerpet_detail_info_t::value_attack)
		.def_readwrite("value_defensive", &CGA::cga_playerpet_detail_info_t::value_defensive)
		.def_readwrite("value_agility", &CGA::cga_playerpet_detail_info_t::value_agility)
		.def_readwrite("value_spirit", &CGA::cga_playerpet_detail_info_t::value_spirit)
		.def_readwrite("value_recovery", &CGA::cga_playerpet_detail_info_t::value_recovery)
		.def_readwrite("resist_poison", &CGA::cga_playerpet_detail_info_t::resist_poison)
		.def_readwrite("resist_sleep", &CGA::cga_playerpet_detail_info_t::resist_sleep)
		.def_readwrite("resist_medusa", &CGA::cga_playerpet_detail_info_t::resist_medusa)
		.def_readwrite("resist_drunk", &CGA::cga_playerpet_detail_info_t::resist_drunk)
		.def_readwrite("resist_chaos", &CGA::cga_playerpet_detail_info_t::resist_chaos)
		.def_readwrite("resist_forget", &CGA::cga_playerpet_detail_info_t::resist_forget)
		.def_readwrite("fix_critical", &CGA::cga_playerpet_detail_info_t::fix_critical)
		.def_readwrite("fix_strikeback", &CGA::cga_playerpet_detail_info_t::fix_strikeback)
		.def_readwrite("fix_accurancy", &CGA::cga_playerpet_detail_info_t::fix_accurancy)
		.def_readwrite("fix_dodge", &CGA::cga_playerpet_detail_info_t::fix_dodge)
		.def_readwrite("element_earth", &CGA::cga_playerpet_detail_info_t::element_earth)
		.def_readwrite("element_water", &CGA::cga_playerpet_detail_info_t::element_water)
		.def_readwrite("element_fire", &CGA::cga_playerpet_detail_info_t::element_fire)
		.def_readwrite("element_wind", &CGA::cga_playerpet_detail_info_t::element_wind)
		;
	py::class_<CGA::cga_pers_desc_t>(m, "cga_pers_desc_t")
		.def(py::init<>())
		.def_readwrite("changeBits", &CGA::cga_pers_desc_t::changeBits)
		.def_readwrite("sellIcon", &CGA::cga_pers_desc_t::sellIcon)
		.def_readwrite("sellString", &CGA::cga_pers_desc_t::sellString)
		.def_readwrite("buyIcon", &CGA::cga_pers_desc_t::buyIcon)
		.def_readwrite("buyString", &CGA::cga_pers_desc_t::buyString)
		.def_readwrite("wantIcon", &CGA::cga_pers_desc_t::wantIcon)
		.def_readwrite("wantString", &CGA::cga_pers_desc_t::wantString)
		.def_readwrite("descString", &CGA::cga_pers_desc_t::descString)
		;
	py::class_<CGA::cga_trade_pet_info_t>(m, "cga_trade_pet_info_t")
		.def(py::init<>())
		.def_readwrite("level", &CGA::cga_trade_pet_info_t::level)
		.def_readwrite("race", &CGA::cga_trade_pet_info_t::race)
		.def_readwrite("maxhp", &CGA::cga_trade_pet_info_t::maxhp)
		.def_readwrite("maxmp", &CGA::cga_trade_pet_info_t::maxmp)
		.def_readwrite("loyality", &CGA::cga_trade_pet_info_t::loyality)
		.def_readwrite("skill_count", &CGA::cga_trade_pet_info_t::skill_count)
		.def_readwrite("image_id", &CGA::cga_trade_pet_info_t::image_id)
		.def_readwrite("index", &CGA::cga_trade_pet_info_t::index)
		.def_readwrite("name", &CGA::cga_trade_pet_info_t::name)
		.def_readwrite("realname", &CGA::cga_trade_pet_info_t::realname)
		.def_readwrite("detail", &CGA::cga_trade_pet_info_t::detail)
		;
	py::class_<CGA::cga_trade_pet_skill_info_t>(m, "cga_trade_pet_skill_info_t")
		.def(py::init<>())
		.def_readwrite("index", &CGA::cga_trade_pet_skill_info_t::index)
		.def_readwrite("skills", &CGA::cga_trade_pet_skill_info_t::skills);
	py::class_<CGA::cga_chat_msg_t>(m, "cga_chat_msg_t")
		.def(py::init<>())
		.def_readwrite("unitid", &CGA::cga_chat_msg_t::unitid)
		.def_readwrite("msg", &CGA::cga_chat_msg_t::msg)
		.def_readwrite("color", &CGA::cga_chat_msg_t::color)
		.def_readwrite("size", &CGA::cga_chat_msg_t::size);
	py::class_<CGA::cga_trade_stuff_t>(m, "cga_trade_stuff_t")
		.def(py::init<>())
		.def_readwrite("items", &CGA::cga_trade_stuff_t::items)
		.def_readwrite("pets", &CGA::cga_trade_stuff_t::pets)
		.def_readwrite("gold", &CGA::cga_trade_stuff_t::gold);
	py::class_<CGA::cga_player_menu_item_t>(m, "cga_player_menu_item_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_player_menu_item_t::name)
		.def_readwrite("index", &CGA::cga_player_menu_item_t::index)
		.def_readwrite("color", &CGA::cga_player_menu_item_t::color);
	py::class_<CGA::cga_unit_menu_item_t>(m, "cga_unit_menu_item_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_unit_menu_item_t::name)
		.def_readwrite("level", &CGA::cga_unit_menu_item_t::level)
		.def_readwrite("health", &CGA::cga_unit_menu_item_t::health)
		.def_readwrite("hp", &CGA::cga_unit_menu_item_t::hp)
		.def_readwrite("maxhp", &CGA::cga_unit_menu_item_t::maxhp)
		.def_readwrite("mp", &CGA::cga_unit_menu_item_t::mp)
		.def_readwrite("maxmp", &CGA::cga_unit_menu_item_t::maxmp)
		.def_readwrite("color", &CGA::cga_unit_menu_item_t::color)
		.def_readwrite("index", &CGA::cga_unit_menu_item_t::index)
		;
	py::class_<CGA::cga_npc_dialog_t>(m, "cga_npc_dialog_t")
		.def(py::init<>())
		.def_readwrite("type", &CGA::cga_npc_dialog_t::type)
		.def_readwrite("options", &CGA::cga_npc_dialog_t::options)
		.def_readwrite("dialog_id", &CGA::cga_npc_dialog_t::dialog_id)
		.def_readwrite("npc_id", &CGA::cga_npc_dialog_t::npc_id)
		.def_readwrite("message", &CGA::cga_npc_dialog_t::message)
		;
	py::class_<CGA::cga_map_unit_t>(m, "cga_map_unit_t")
		.def(py::init<>())
		.def_readwrite("valid", &CGA::cga_map_unit_t::valid)
		.def_readwrite("type", &CGA::cga_map_unit_t::type)
		.def_readwrite("unit_id", &CGA::cga_map_unit_t::unit_id)
		.def_readwrite("model_id", &CGA::cga_map_unit_t::model_id)
		.def_readwrite("xpos", &CGA::cga_map_unit_t::xpos)
		.def_readwrite("ypos", &CGA::cga_map_unit_t::ypos)
		.def_readwrite("item_count", &CGA::cga_map_unit_t::item_count)
		.def_readwrite("injury", &CGA::cga_map_unit_t::injury)
		.def_readwrite("icon", &CGA::cga_map_unit_t::icon)
		.def_readwrite("level", &CGA::cga_map_unit_t::level)
		.def_readwrite("flags", &CGA::cga_map_unit_t::flags)
		.def_readwrite("unit_name", &CGA::cga_map_unit_t::unit_name)
		.def_readwrite("nick_name", &CGA::cga_map_unit_t::nick_name)
		.def_readwrite("title_name", &CGA::cga_map_unit_t::title_name)
		.def_readwrite("item_name", &CGA::cga_map_unit_t::item_name)

		;

	py::class_<CGA::cga_working_result_t>(m, "cga_working_result_t")
		.def(py::init<>())
		.def_readwrite("type", &CGA::cga_working_result_t::type)
		.def_readwrite("success", &CGA::cga_working_result_t::success)
		.def_readwrite("levelup", &CGA::cga_working_result_t::levelup)
		.def_readwrite("xp", &CGA::cga_working_result_t::xp)
		.def_readwrite("count", &CGA::cga_working_result_t::count)
		.def_readwrite("endurance", &CGA::cga_working_result_t::endurance)
		.def_readwrite("skillful", &CGA::cga_working_result_t::skillful)
		.def_readwrite("intelligence", &CGA::cga_working_result_t::intelligence)
		.def_readwrite("name", &CGA::cga_working_result_t::name)
		.def_readwrite("value", &CGA::cga_working_result_t::value)
		.def_readwrite("imgid", &CGA::cga_working_result_t::imgid)
		.def_readwrite("status", &CGA::cga_working_result_t::status)
		.def_readwrite("value2", &CGA::cga_working_result_t::value2)
		.def_readwrite("count", &CGA::cga_working_result_t::count)
		.def_readwrite("unk", &CGA::cga_working_result_t::unk)
		;
	py::class_<CGA::cga_craft_material_t>(m, "cga_craft_material_t")
		.def(py::init<>())
		.def_readwrite("itemid", &CGA::cga_craft_material_t::itemid)
		.def_readwrite("count", &CGA::cga_craft_material_t::count)
		.def_readwrite("name", &CGA::cga_craft_material_t::name)
		;
	py::class_<pcga_craft_info_t>(m, "cga_craft_info_t")
		.def(py::init<>())
		.def_readwrite("id", &pcga_craft_info_t::id)
		.def_readwrite("cost", &pcga_craft_info_t::cost)
		.def_readwrite("level", &pcga_craft_info_t::level)
		.def_readwrite("itemid", &pcga_craft_info_t::itemid)
		.def_readwrite("index", &pcga_craft_info_t::index)
		.def_readwrite("name", &pcga_craft_info_t::name)
		.def_readwrite("info", &pcga_craft_info_t::info)
		.def_readwrite("materials", &pcga_craft_info_t::materials)
		.def_readwrite("available", &pcga_craft_info_t::available)
		;
	py::class_<CGA::cga_trade_stuff_info_t>(m, "cga_trade_stuff_info_t")
		.def(py::init<>())
		.def_readwrite("items", &CGA::cga_trade_stuff_info_t::items)
		.def_readwrite("pet", &CGA::cga_trade_stuff_info_t::pet)
		.def_readwrite("petskills", &CGA::cga_trade_stuff_info_t::petskills)
		.def_readwrite("gold", &CGA::cga_trade_stuff_info_t::gold)
		.def_readwrite("type", &CGA::cga_trade_stuff_info_t::type)
		;
	py::class_<CGA::cga_team_player_t>(m, "cga_team_player_t")
		.def(py::init<>())
		.def_readwrite("unit_id", &CGA::cga_team_player_t::unit_id)
		.def_readwrite("hp", &CGA::cga_team_player_t::hp)
		.def_readwrite("maxhp", &CGA::cga_team_player_t::maxhp)
		.def_readwrite("mp", &CGA::cga_team_player_t::mp)
		.def_readwrite("maxmp", &CGA::cga_team_player_t::maxmp)
		.def_readwrite("xpos", &CGA::cga_team_player_t::xpos)
		.def_readwrite("ypos", &CGA::cga_team_player_t::ypos)
		.def_readwrite("name", &CGA::cga_team_player_t::name)
		;
	py::class_<CGA::cga_map_cells_t>(m, "cga_map_cells_t")
		.def(py::init<>())
		.def_readwrite("x_bottom", &CGA::cga_map_cells_t::x_bottom)
		.def_readwrite("y_bottom", &CGA::cga_map_cells_t::y_bottom)
		.def_readwrite("x_size", &CGA::cga_map_cells_t::x_size)
		.def_readwrite("y_size", &CGA::cga_map_cells_t::y_size)
		.def_readwrite("filename", &CGA::cga_map_cells_t::filename)
		.def_readwrite("cell", &CGA::cga_map_cells_t::cell)
		;
	py::class_<CGA::cga_trade_dialog_t>(m, "cga_trade_dialog_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_trade_dialog_t::name)
		.def_readwrite("level", &CGA::cga_trade_dialog_t::level)
		;
	py::class_<CGA::cga_battle_context_t>(m, "cga_battle_context_t")
		.def(py::init<>())
		.def_readwrite("round_count", &CGA::cga_battle_context_t::round_count)
		.def_readwrite("player_pos", &CGA::cga_battle_context_t::player_pos)
		.def_readwrite("player_status", &CGA::cga_battle_context_t::player_status)
		.def_readwrite("skill_performed", &CGA::cga_battle_context_t::skill_performed)
		.def_readwrite("skill_allowbit", &CGA::cga_battle_context_t::skill_allowbit)
		.def_readwrite("petskill_allowbit", &CGA::cga_battle_context_t::petskill_allowbit)
		.def_readwrite("weapon_allowbit", &CGA::cga_battle_context_t::weapon_allowbit)
		.def_readwrite("petid", &CGA::cga_battle_context_t::petid)
		.def_readwrite("effect_flags", &CGA::cga_battle_context_t::effect_flags)
		;
	py::class_<CGA::cga_download_map_t>(m, "cga_download_map_t")
		.def(py::init<>())
		.def_readwrite("index1", &CGA::cga_download_map_t::index1)
		.def_readwrite("index3", &CGA::cga_download_map_t::index3)
		.def_readwrite("xbase", &CGA::cga_download_map_t::xbase)
		.def_readwrite("ybase", &CGA::cga_download_map_t::ybase)
		.def_readwrite("xtop", &CGA::cga_download_map_t::xtop)
		.def_readwrite("ytop", &CGA::cga_download_map_t::ytop)
		;
	py::class_<CGA::cga_conn_state_t>(m, "cga_conn_state_t")
		.def(py::init<>())
		.def_readwrite("state", &CGA::cga_conn_state_t::state)
		.def_readwrite("msg", &CGA::cga_conn_state_t::msg)
		;
	py::class_<CGA::cga_item_info_t>(m, "cga_item_info_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_item_info_t::name)
		.def_readwrite("attr", &CGA::cga_item_info_t::attr)
		.def_readwrite("info", &CGA::cga_item_info_t::info)
		.def_readwrite("itemid", &CGA::cga_item_info_t::itemid)
		.def_readwrite("count", &CGA::cga_item_info_t::count)
		.def_readwrite("pos", &CGA::cga_item_info_t::pos)
		.def_readwrite("level", &CGA::cga_item_info_t::level)
		.def_readwrite("type", &CGA::cga_item_info_t::type)
		.def_readwrite("assessed", &CGA::cga_item_info_t::assessed)
		.def_readwrite("assess_flags", &CGA::cga_item_info_t::assess_flags)
		.def_readwrite("image_id", &CGA::cga_item_info_t::image_id);
	py::class_<CGA::cga_battle_unit_t>(m, "cga_battle_unit_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_battle_unit_t::name)
		.def_readwrite("modelid", &CGA::cga_battle_unit_t::modelid)
		.def_readwrite("curhp", &CGA::cga_battle_unit_t::curhp)
		.def_readwrite("maxhp", &CGA::cga_battle_unit_t::maxhp)
		.def_readwrite("curmp", &CGA::cga_battle_unit_t::curmp)
		.def_readwrite("maxmp", &CGA::cga_battle_unit_t::maxmp)
		.def_readwrite("level", &CGA::cga_battle_unit_t::level)
		.def_readwrite("flags", &CGA::cga_battle_unit_t::flags)
		.def_readwrite("pos", &CGA::cga_battle_unit_t::pos)
		.def_readwrite("petriding_modelid", &CGA::cga_battle_unit_t::petriding_modelid);

	py::class_<CGA::cga_skill_info_t>(m, "cga_skill_info_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_skill_info_t::name)
		.def_readwrite("lv", &CGA::cga_skill_info_t::lv)
		.def_readwrite("maxlv", &CGA::cga_skill_info_t::maxlv)
		.def_readwrite("xp", &CGA::cga_skill_info_t::xp)
		.def_readwrite("maxxp", &CGA::cga_skill_info_t::maxxp)
		.def_readwrite("skill_id", &CGA::cga_skill_info_t::skill_id)
		.def_readwrite("type", &CGA::cga_skill_info_t::type)
		.def_readwrite("pos", &CGA::cga_skill_info_t::pos)
		.def_readwrite("index", &CGA::cga_skill_info_t::index)
		.def_readwrite("slotsize", &CGA::cga_skill_info_t::slotsize)
		;
	py::class_<CGA::cga_subskill_info_t>(m, "cga_subskill_info_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_subskill_info_t::name)
		.def_readwrite("cost", &CGA::cga_subskill_info_t::cost)
		.def_readwrite("info", &CGA::cga_subskill_info_t::info)
		.def_readwrite("flags", &CGA::cga_subskill_info_t::flags)
		.def_readwrite("level", &CGA::cga_subskill_info_t::level)
		.def_readwrite("available", &CGA::cga_subskill_info_t::available)
		;
	py::class_<CGA::cga_sell_item_t>(m, "cga_sell_item_t")
		.def(py::init<>())
		.def_readwrite("itemid", &CGA::cga_sell_item_t::itemid)
		.def_readwrite("itempos", &CGA::cga_sell_item_t::itempos)
		.def_readwrite("count", &CGA::cga_sell_item_t::count)
		;
	py::class_< std::vector<CGA::cga_sell_item_t>>(m, "cga_sell_items_t")
		.def(py::init<>())
		.def("append",
			[](std::vector<cga_sell_item_t>& vec, cga_sell_item_t value) { vec.emplace_back(value); })
		.def("__iter__", [](std::vector<cga_sell_item_t>& vec) {
		return py::make_iterator(vec.begin(), vec.end());
			}, py::keep_alive<0, 1>());
	py::class_<CGA::cga_buy_item_t>(m, "cga_buy_item_t")
		.def(py::init<>())
		.def_readwrite("index", &CGA::cga_buy_item_t::index)
		.def_readwrite("count", &CGA::cga_buy_item_t::count)
		;
	py::class_<TMLPoint>(m, "TMLPoint")
		.def(py::init<>())
		.def_readwrite("x", &TMLPoint::x)
		.def_readwrite("y", &TMLPoint::y)
		;
	py::class_<pcga_craft_item_t>(m, "cga_craft_item_t")
		.def(py::init<>())
		.def_readwrite("skill_index", &pcga_craft_item_t::skill_index)
		.def_readwrite("subskill_index", &pcga_craft_item_t::subskill_index)
		.def_readwrite("sub_type", &pcga_craft_item_t::sub_type)
		.def_readwrite("itempos", &pcga_craft_item_t::itempos)
		;
	py::class_<CGA::cga_pet_skill_info_t>(m, "cga_pet_skill_info_t")
		.def(py::init<>())
		.def_readwrite("name", &CGA::cga_pet_skill_info_t::name)
		.def_readwrite("cost", &CGA::cga_pet_skill_info_t::cost)
		.def_readwrite("info", &CGA::cga_pet_skill_info_t::info)
		.def_readwrite("flags", &CGA::cga_pet_skill_info_t::flags)
		.def_readwrite("index", &CGA::cga_pet_skill_info_t::index)
		;
	py::class_< std::vector<CGA::cga_pet_skill_info_t>>(m, "cga_pet_skills_info_t")
		.def(py::init<>())
		.def("append",
			[](std::vector<cga_pet_skill_info_t>& vec, cga_pet_skill_info_t value) { vec.emplace_back(value); })
		.def("__iter__", [](std::vector<cga_pet_skill_info_t>& vec) {
		return py::make_iterator(vec.begin(), vec.end());
	});

	//py::bind_vector<std::vector<cga_pet_skill_info_t>>(m, "cga_pet_skills_info_t", py::module_local(false));
	//py::bind_vector<std::vector<int>>(m, "VectorInt");
	/*py::class_<std::vector<int>>(m, "IntVector")
		.def(py::init<>())
		.def("append",
			[](std::vector<int>& v, int value) { v.emplace_back(value); })
		.def("clear", &std::vector<int>::clear)
		.def("pop_back", &std::vector<int>::pop_back)
		.def("__len__", [](const std::vector<int>& v) { return v.size(); })
		.def("__iter__", [](std::vector<int>& v) {
		return py::make_iterator(v.begin(), v.end());
	}, py::keep_alive<0, 1>());*/
	//py::add_ostream_redirect(m, "ostream_redirect");

	py::class_<CGAPython>(m, "CGA")
		.def(py::init<>())
		.def("IsConnected", &CGAPython::IsConnected)
		.def("Disconnect", &CGAPython::Disconnect)
		.def("Connect", &CGAPython::Connect)
		.def("Initialize", &CGAPython::Initialize)
		.def("IsInGame", &CGAPython::IsInGame)
		.def("GetWorldStatus", &CGAPython::GetWorldStatus)
		.def("GetGameStatus", &CGAPython::GetGameStatus)
		.def("GetBGMIndex", &CGAPython::GetBGMIndex)
		.def("GetSysTime", &CGAPython::GetSysTime)
		.def("GetPlayerInfo", &CGAPython::GetPlayerInfo)
		.def("SetPlayerFlagEnabled", &CGAPython::SetPlayerFlagEnabled)
		.def("IsPlayerFlagEnabled", &CGAPython::IsPlayerFlagEnabled)

		.def("IsPetValid", &CGAPython::IsPetValid)
		.def("GetPetInfo", &CGAPython::GetPetInfo)
		.def("GetPetsInfo", &CGAPython::GetPetsInfo)
		.def("GetBankPetsInfo", &CGAPython::GetBankPetsInfo)
		.def("IsPetSkillValid", &CGAPython::IsPetSkillValid)
		.def("GetPetSkillInfo", &CGAPython::GetPetSkillInfo)
		.def("GetPetSkillsInfo", &CGAPython::GetPetSkillsInfo)
		//.def("GetPetSkillsInfo1", &CGAPython::GetPetSkillsInfo1)
		.def("IsSkillValid", &CGAPython::IsSkillValid)
		.def("GetSkillInfo", &CGAPython::GetSkillInfo)
		.def("GetSkillsInfo", &CGAPython::GetSkillsInfo)
		.def("GetSubSkillInfo", &CGAPython::GetSubSkillInfo)
		.def("GetSubSkillsInfo", &CGAPython::GetSubSkillsInfo)
		.def("GetPicBooksInfo", &CGAPython::GetPicBooksInfo)
		.def("GetCardsInfo", &CGAPython::GetCardsInfo)
		.def("GetCardsRecvMsg", &CGAPython::GetCardsRecvMsg)
		.def("SetCardRecvMsgState", &CGAPython::SetCardRecvMsgState)
		.def("IsItemValid", &CGAPython::IsItemValid)
		.def("GetItemInfo", &CGAPython::GetItemInfo)
		.def("GetItemsInfo", &CGAPython::GetItemsInfo)
		.def("GetBankItemsInfo", &CGAPython::GetBankItemsInfo)
		.def("GetBankGold", &CGAPython::GetBankGold)
		.def("MoveItem", &CGAPython::MoveItem)
		.def("MovePet", &CGAPython::MovePet)
		.def("MoveGold", &CGAPython::MoveGold)
		.def("DropItem", &CGAPython::DropItem)
		.def("DropPet", &CGAPython::DropPet)
		.def("ChangePetState", &CGAPython::ChangePetState)
		.def("GetMapIndex", &CGAPython::GetMapIndex)
		.def("GetMapXY", &CGAPython::GetMapXY)
		.def("GetMapCoordinate", &CGAPython::GetMapCoordinate)
		.def("GetMapXYFloat", &CGAPython::GetMapXYFloat)
		.def("GetMoveSpeed", &CGAPython::GetMoveSpeed)
		.def("GetMouseXY", &CGAPython::GetMouseXY)
		.def("GetMouseOrientation", &CGAPython::GetMouseOrientation)
		.def("GetMapName", &CGAPython::GetMapName)
		.def("GetMapUnits", &CGAPython::GetMapUnits)
		.def("GetMapCollisionTable", &CGAPython::GetMapCollisionTable)
		.def("GetMapCollisionTableRaw", &CGAPython::GetMapCollisionTableRaw)
		.def("GetMapObjectTable", &CGAPython::GetMapObjectTable)
		.def("GetMapTileTable", &CGAPython::GetMapTileTable)
		.def("WalkTo", &CGAPython::WalkTo)
		.def("TurnTo", &CGAPython::TurnTo)
		.def("SetMoveSpeed", &CGAPython::SetMoveSpeed)
		.def("ForceMove", &CGAPython::ForceMove)
		.def("ForceMoveTo", &CGAPython::ForceMoveTo)
		.def("IsMapCellPassable", &CGAPython::IsMapCellPassable)
		.def("LogBack", &CGAPython::LogBack)
		.def("LogOut", &CGAPython::LogOut)
		.def("BackSelectServer", &CGAPython::BackSelectServer)
		.def("SayWords", &CGAPython::SayWords)
		.def("ChangeNickName", &CGAPython::ChangeNickName)
		.def("ChangeTitleName", &CGAPython::ChangeTitleName)
		.def("ChangePersDesc", &CGAPython::ChangePersDesc)
		.def("ChangePetName", &CGAPython::ChangePetName)
		.def("UseItem", &CGAPython::UseItem)
		.def("ClickNPCDialog", &CGAPython::ClickNPCDialog)
		.def("SellNPCStore", &CGAPython::SellNPCStore)
		.def("BuyNPCStore", &CGAPython::BuyNPCStore)
		.def("PlayerMenuSelect", &CGAPython::PlayerMenuSelect)
		.def("UnitMenuSelect", &CGAPython::UnitMenuSelect)
		.def("UpgradePlayer", &CGAPython::UpgradePlayer)
		.def("UpgradePet", &CGAPython::UpgradePet)
		.def("IsBattleUnitValid", &CGAPython::IsBattleUnitValid)
		.def("GetBattleUnit", &CGAPython::GetBattleUnit)
		.def("GetBattleUnits", &CGAPython::GetBattleUnits)
		.def("GetBattleContext", &CGAPython::GetBattleContext)
		.def("BattleNormalAttack", &CGAPython::BattleNormalAttack)
		.def("BattleSkillAttack", &CGAPython::BattleSkillAttack)
		.def("BattleRebirth", &CGAPython::BattleRebirth)
		.def("BattleGuard", &CGAPython::BattleGuard)
		.def("BattleEscape", &CGAPython::BattleEscape)
		.def("BattleExchangePosition", &CGAPython::BattleExchangePosition)
		.def("BattleChangePet", &CGAPython::BattleChangePet)
		.def("BattleUseItem", &CGAPython::BattleUseItem)
		.def("BattlePetSkillAttack", &CGAPython::BattlePetSkillAttack)
		.def("BattleDoNothing", &CGAPython::BattleDoNothing)
		.def("BattleSetHighSpeedEnabled", &CGAPython::BattleSetHighSpeedEnabled)
		.def("SetGameTextUIEnabled", &CGAPython::SetGameTextUIEnabled)
		.def("SetGameTextUICurrentScript", &CGAPython::SetGameTextUICurrentScript)
		.def("GetBattleEndTick", &CGAPython::GetBattleEndTick)
		.def("SetBattleEndTick", &CGAPython::SetBattleEndTick)
		.def("SetWorkDelay", &CGAPython::SetWorkDelay)
		.def("SetWorkAcceleration", &CGAPython::SetWorkAcceleration)
		.def("SetImmediateDoneWork", &CGAPython::SetImmediateDoneWork)
		.def("GetImmediateDoneWorkState", &CGAPython::GetImmediateDoneWorkState)
		.def("StartWork", &CGAPython::StartWork)
		.def("CraftItem", &CGAPython::CraftItem)
		.def("AssessItem", &CGAPython::AssessItem)
		.def("GetCraftInfo", &CGAPython::GetCraftInfo)
		.def("GetCraftsInfo", &CGAPython::GetCraftsInfo)
		.def("GetCraftStatus", &CGAPython::GetCraftStatus)
		.def("DoRequest", &CGAPython::DoRequest)
		.def("EnableFlags", &CGAPython::EnableFlags)
		.def("TradeAddStuffs", &CGAPython::TradeAddStuffs)
		.def("AddAllTradeItems", &CGAPython::AddAllTradeItems)
		.def("GetTeamPlayerInfo", &CGAPython::GetTeamPlayerInfo)
		.def("FixMapWarpStuck", &CGAPython::FixMapWarpStuck)
		.def("SetNoSwitchAnim", &CGAPython::SetNoSwitchAnim)
		.def("SetSwitchAnimForceWait", &CGAPython::SetSwitchAnimForceWait)
		.def("SetWindowResolution", &CGAPython::SetWindowResolution)
		.def("RequestDownloadMap", &CGAPython::RequestDownloadMap)
		.def("GetNextAnimTickCount", &CGAPython::GetNextAnimTickCount)
		.def("LoginGameServer", &CGAPython::LoginGameServer)
		.def("CreateCharacter", &CGAPython::CreateCharacter)
		.def("IsUIDialogPresent", &CGAPython::IsUIDialogPresent)
		.def("PlayGesture", &CGAPython::PlayGesture)
		.def("DeleteCard", &CGAPython::DeleteCard)
		.def("SendMail", &CGAPython::SendMail)
		.def("SendPetMail", &CGAPython::SendPetMail)
		.def("GetGameServerInfo", &CGAPython::GetGameServerInfo)
		.def("AutoMoveTo", &CGAPython::AutoMoveToEx, "自动寻路,参数(x,y,name,timeout),超时可选,单位毫秒", py::arg("x"), py::arg("y"), py::arg("sMapName")="", py::arg("timeout") = 10000)
		.def("InitCaclPetData", &CGAPython::initCaclPetData)
		.def("ParsePetGrade", &CGAPython::ParsePetGrade)

		.def("RegisterChatMsgNotify", &CGAPython::RegisterChatMsgNotify)
		.def("RegisterServerShutdownNotify", &CGAPython::RegisterServerShutdownNotify)
		.def("RegisterGameWndKeyDownNotify", &CGAPython::RegisterGameWndKeyDownNotify)
		.def("RegisterBattleActionNotify", &CGAPython::RegisterBattleActionNotify)
		.def("RegisterPlayerMenuNotify", &CGAPython::RegisterPlayerMenuNotify)
		.def("RegisterNPCDialogNotify", &CGAPython::RegisterNPCDialogNotify)
		.def("RegisterWorkingResultNotify", &CGAPython::RegisterWorkingResultNotify)
		.def("RegisterTradeStuffsNotify", &CGAPython::RegisterTradeStuffsNotify)
		.def("RegisterTradeDialogNotify", &CGAPython::RegisterTradeDialogNotify)
		.def("RegisterTradeStateNotify", &CGAPython::RegisterTradeStateNotify)
		.def("RegisterDownloadMapNotify", &CGAPython::RegisterDownloadMapNotify)
		.def("RegisterConnectionStateNotify", &CGAPython::RegisterConnectionStateNotify)
		.def("RegisterUnitMenuNotify", &CGAPython::RegisterUnitMenuNotify)
		;

}