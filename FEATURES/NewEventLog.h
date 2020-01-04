#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "..\SDK\GameEvents.h"

#define CREATE_EVENT_LISTENER(class_name)\
class class_name : public SDK::IGameEventListener\
{\
public:\
	~class_name() { INTERFACES::GameEventManager->RemoveListener(this); }\
	virtual void FireGameEvent(SDK::IGameEvent* game_event);\
};

namespace CGameEvents
{
	void InitializeEventListeners();
	CREATE_EVENT_LISTENER(ItemPurchaseListener);
	CREATE_EVENT_LISTENER(PlayerHurtListener);
	CREATE_EVENT_LISTENER(RoundStartListener);
	CREATE_EVENT_LISTENER(BulletImpactListener);
	CREATE_EVENT_LISTENER(PlayerDeathListener);
	CREATE_EVENT_LISTENER(WeaponFireListener);
	CREATE_EVENT_LISTENER(RoundPreStartListener);
	CREATE_EVENT_LISTENER(RoundEndListener);
};

namespace FEATURES
{

	namespace MISC
	{
		class ColorLine
		{
		public:
			ColorLine() {}

			ColorLine(std::string text, CColor color = CColor(255, 255, 255, 255))
			{
				texts.push_back(text);
				colors.push_back(color);
			}

			void PushBack(std::string text, CColor color = CColor(255, 255, 255, 255))
			{
				texts.push_back(text);
				colors.push_back(color);
			}

			void PushFront(std::string text, CColor color = CColor(255, 255, 255, 255))
			{
				texts.insert(texts.begin(), text);
				colors.insert(colors.begin(), color);
			}

			void ChangeAlpha(int alpha)
			{
				for (auto& color : colors)
					color.RGBA[3] = alpha;
			}

			void Draw(int x, int y, unsigned int font);

			std::string Text()
			{
				std::string text;
				for (const auto string : texts)
					text += string;

				return text;
			}

		private:
			std::vector<std::string> texts;
			std::vector<CColor> colors;
		};

		class InGameLogger
		{
		public:
			struct Log
			{
				Log()
				{
					time = UTILS::GetCurtime();
				}

				Log(std::string text, CColor color = CColor(255, 255, 255, 255))
				{
					color_line = ColorLine(text, color);
					time = UTILS::GetCurtime();
				}

				ColorLine color_line;
				float time;
			};

		public:
			void print(std::string prefix);
			void Do();

			void AddLog(Log log)
			{
				log_queue.insert(log_queue.begin(), log);
			}

		private:
			std::vector<Log> log_queue;

		private:
			const float text_time = 5.f;
			const float text_fade_in_time = 0.3f;
			const float text_fade_out_time = 0.2f;

			const int max_lines_at_once = 10;

			const int ideal_height = 20;
			const int ideal_width = 20;

			const int slide_in_distance = 20;
			const int slide_out_distance = 20;
			const float slide_out_speed = 0.2f;
		};

		extern InGameLogger in_game_logger;
	}
}

class sound_info
{
public:
	sound_info(Vector positions, float times, int userids)
	{
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	Vector position;
	float time;
	int userid;
};

class soundesp
{
public:
	void draw();
	void draw_circle(CColor color, Vector position);
};

extern soundesp g_soundesp;
class NEL {
public:
	struct LagComp
	{
		float flEraseTime;
		SDK::CBaseEntity * Player;
		matrix3x4_t pmatrix[128];
	};
	std::vector<LagComp>lag_comp;
};
extern NEL* nel;
class bullettracers
{
private:
	class trace_info {
	public:
		trace_info(Vector starts, Vector positions, float times) {
			this->start = starts;
			this->position = positions;
			this->time = times;
		}

		Vector position;
		Vector start;
		float time;
	};

	std::vector<trace_info> logs;

	void draw_beam(Vector src, Vector end, CColor color);
	void draw_beam1(Vector src, Vector end, CColor color);
public:
	void events(SDK::IGameEvent* event);
};

extern bullettracers impact;
namespace FEATURES
{
	namespace MISC
	{
		void InitializeEventListeners();
		void RemoveEventListeners();
		void Do();
	}
}

struct glow {
	VMatrix playerMatrix[128];
};