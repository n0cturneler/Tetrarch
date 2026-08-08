#include "UI.hpp"

#include "Pieces.hpp"
#include "Bag.hpp"
#include "Options.hpp"
using namespace options;

#include <raylib.h>

#include <imgui.h>
#include <rlImGuiColors.h>

#include <chrono>
#include <format>
#include <string>
#include <string_view>
#include <iostream>

void UI::initialize()
{
    ImGuiIO& io = ImGui::GetIO();

    UI::mainFont = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/Space_Grotesk.ttf",
        20.0f
    );

    UI::mainFontBig = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/Space_Grotesk.ttf",
        48.0f
    );

    UI::mainFontSmall = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/Space_Grotesk.ttf",
        12.0f
    );

    UI::tetrisFontMedium = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/TetrisBlocks.ttf",
        64.0f
    );

    io.FontDefault = mainFont;
}

void UI::FPS()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f),
                            ImGuiCond_Always,
                            ImVec2(0.0f, 0.0f)); // anchor pos

    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(GetScreenWidth()) / 30.0f, static_cast<float>(GetScreenHeight()) / 15.0f), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(colors::textLight));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, rlImGuiColors::Convert(colors::uiTitleActive));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, rlImGuiColors::Convert(colors::uiTitleActive));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, rlImGuiColors::Convert(colors::uiBG));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

    ImGui::Begin(
        "FPS",
        nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::Text("%.2f", 1.0f / GetFrameTime());

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);

    ImGui::End();
}

void UI::lockDelay(const piece::Piece& activePiece, const board::Board& curBoard)
{   
    using ms = std::chrono::milliseconds;
    auto now{std::chrono::steady_clock::now()};

	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(GetScreenWidth()) / 2.0f, // x
                            static_cast<float>(GetScreenHeight()) - (static_cast<float>(GetScreenHeight()) / 10.0f)), // y
                            ImGuiCond_Always, 
                            ImVec2(0.5f, 0.5f)); // anchor pos

    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(GetScreenWidth()) / 4.0f, static_cast<float>(GetScreenHeight()) / 22.0f), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(colors::textLight));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, rlImGuiColors::Convert(colors::uiTitleActive));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, rlImGuiColors::Convert(colors::uiTitleActive));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, rlImGuiColors::Convert(colors::uiBG));

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, rlImGuiColors::Convert(colors::uiALT1));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

    ImGui::PushFont(UI::mainFontSmall);

	ImGui::Begin(
        "Lock Delay",
        nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(colors::uiALT1));

    float duration_lock{static_cast<float>(std::chrono::duration_cast<ms>(now - activePiece.lockStart()).count())};
    float tProgress{duration_lock / static_cast<float>(game::lockDelayMS)};

    std::string text{};
    std::cout << activePiece.gridPos().x << ", " << activePiece.gridPos().y + 1 << '\n';

    if (!activePiece.isPositionValid(curBoard, {0, 1}))
    {
        text = std::format("[{}ms]", duration_lock);
    } 
    else
    {   
        text = std::format("[{}ms]", 0.0f);
        tProgress = 0.0f;
    }

    ImGui::ProgressBar(tProgress, ImVec2(-1.0f, -1.0f), text.c_str());
    ImGui::PopFont();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(6);
    
    ImGui::End();
}

void UI::drawBag(bag::Bag& currentBag, bag::Bag& nextBag)
{
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(GetScreenWidth()) / 1.6f,
                                   static_cast<float>(GetScreenHeight()) / 6.0f),
                            ImGuiCond_Always,
                            ImVec2(0.0f, 0.0f)); // anchor pos

    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(GetScreenWidth()) / 15.0f, static_cast<float>(GetScreenHeight()) / 1.8f), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(colors::textLight));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, rlImGuiColors::Convert(colors::uiTitleActive));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, rlImGuiColors::Convert(colors::uiTitleActive));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, rlImGuiColors::Convert(colors::uiBG));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

    ImGui::Begin(
        "Bag",
        nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::Text("Next Pieces:");

    ImGui::BeginTable(" ", 1);

    ImGui::PushFont(UI::tetrisFontMedium);
    for (std::size_t i{0}; i < 7; ++i)
    {   
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        if (i >= currentBag.currentIndex())
        {   
            ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(colors::piece[static_cast<std::size_t>(currentBag.data()[i])]));

            std::string text = std::string(textToTetrisFont(pieceType::pieceTypeToString(currentBag.data()[i])));
            ImGui::Text("%s", text.c_str());

            ImGui::PopStyleColor();
        }
    }

    for (std::size_t i{0}; i < currentBag.currentIndex(); ++i)
    {   
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::string text = std::string(textToTetrisFont(pieceType::pieceTypeToString(nextBag.data()[i])));

        ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(colors::piece[static_cast<std::size_t>(nextBag.data()[i])]));
        ImGui::Text("%s", text.c_str());

        ImGui::PopStyleColor();
    }

    ImGui::PopFont();   

    ImGui::EndTable();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);

    ImGui::End();
}

std::string_view UI::textToTetrisFont(std::string_view text)
{   
    if (text == "I") return "G";
    if (text == "J") return "N";
    if (text == "L") return "O";
    if (text == "O") return "B";
    if (text == "S") return "K";
    if (text == "T") return "C";
    if (text == "Z") return "H";
    if (text == "X") return "T";

    return "???";
}
