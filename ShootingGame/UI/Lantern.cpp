#include "Lantern.h"
#include <Engine/Engine.h>
#include <Render/Renderer.h>



void Lantern::BeginPlay()
{
}

void Lantern::Tick(float deltaTime)
{
	lanternFrameTimer += deltaTime;
	if (lanternFrameTimer >= lanternFrameDuration)
	{
		lanternFrameTimer -= lanternFrameDuration;
		lanternFrame = (lanternFrame + 1) % 4;
	}
}

void Lantern::Draw()
{
	static const std::vector <std::string> lanternFrames[4] =
	{
		{
			"      )  (   )     ",
			"    (   ) (  '  )  ",
			"   ( , ') . (  , ) ",
			"    '-.,_,.-'  ,'  ",
			"       ;;|;;       ",
			"        |=|        ",
			"        |:|        ",
			"        |=|        ",
			"        |:|        ",
			"        `-'        "
		},
		{
			"    (   )  (       ",
			"   (  ' ) (   )    ",
			"  ( . ,') ( , )    ",
			"   ','-.,_,.-'     ",
			"       ;:|:;       ",
			"        |=|        ",
			"        |:|        ",
			"        |=|        ",
			"        |:|        ",
			"        `-'        "
		},
		{
			"     (  )   (      ",
			"    )  ( ' )  )    ",
			"   ( ,' ) . ( ,)   ",
			"    '-.,_,.-' '    ",
			"       :;|;:       ",
			"        |=|        ",
			"        |:|        ",
			"        |=|        ",
			"        |:|        ",
			"        `-'        "
		},
		{
			"      (  )  )      ",
			"     ( ) ('  (     ",
			"    ( ,) . (', )   ",
			"     ,'-.,_,.-'    ",
			"       ;;|;;       ",
			"        |=|        ",
			"        |:|        ",
			"        |=|        ",
			"        |:|        ",
			"        `-'        "
		}
		};
		Renderer::Get().Submit(lanternFrames[lanternFrame], GetPosition(), Craft::Color::Yellow);
}



