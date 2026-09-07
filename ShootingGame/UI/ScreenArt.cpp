#include "ScreenArt.h"

std::vector<std::string> ScreenArt::GetArt(ScreenType type) const
{
    switch (type)
    {
    case ScreenType::MainScreen:
        return{

        };
    case ScreenType::Win:
        return {
        " #####  #       #######    #    ######  ### ",
        "#     # #       #         # #   #     # ### ",
        "#       #       #        #   #  #     # ### ",
        "#       #       #####   #     # ######   #  ",
        "#       #       #       ####### #   #       ",
        "#     # #       #       #     # #    #  ### ",
        " #####  ####### ####### #     # #     # ### "

};
    case ScreenType::GameOver:
        return {

        };
    }
    return std::vector<std::string>();
}
