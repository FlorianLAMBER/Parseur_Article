#include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <memory>    // for shared_ptr, __shared_ptr_access
#include <string>    // for to_string, allocator

#include <iostream>  
#include <string>  
#include <vector>  
#include <sstream>  
#include <fstream>

#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Checkbox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, vscroll_indicator, HEIGHT, LESS_THAN

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace ftxui;

// Define a special style for some menu entry.
MenuEntryOption Colored(ftxui::Color c) {
  MenuEntryOption option;
  option.animated_colors.foreground.enabled = true;
  option.animated_colors.background.enabled = true;
  option.animated_colors.background.active = c;
  option.animated_colors.background.inactive = Color::Black;
  option.animated_colors.foreground.active = Color::White;
  option.animated_colors.foreground.inactive = c;
  return option;
}

/**
 * @brief Execute a commande
 * 
 * @param const char* Commande to execute 
 * @return std::string Output of the commande
 */
std::string exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    //if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
		//std::cout << "ERROR in bufferisation" << std::endl;
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}



/**
 * @brief Update the list pdf list
 */
void update_List(std::string path, std::vector<std::string>*  pdfList)
{
  
  pdfList->clear();pdfList->push_back("..");
	std::string cmd = "ls -l  \"" +  path + "\" | egrep '^d' | awk '{print $NF}'";
  //std::string cmd = "find  \"" +  path + "\" -maxdepth 1 -type d";
	std::string cmd_output = exec(cmd.c_str());

	// See getline(X,T, ' ')
	std::string line;
	std::stringstream stream_cmd_output(cmd_output);
	while(getline(stream_cmd_output, line)){
		pdfList->push_back(line);
	}

}

/**
 * @brief Update the list pdf list
 */
void update_pdfList(std::string path, std::vector<std::string>*  pdfList)
{
  
  pdfList->clear();pdfList->push_back("All");
	std::string cmd = "ls -l  \"" +  path + "\" | grep .pdf | awk '{print $NF}'";
  //std::string cmd = "find  \"" +  path + "\" -maxdepth 1 -type d";
	std::string cmd_output = exec(cmd.c_str());

	// See getline(X,T, ' ')
	std::string line;
	std::stringstream stream_cmd_output(cmd_output);
	while(getline(stream_cmd_output, line)){
		pdfList->push_back(line);
	}

}

// Display a component nicely with a title on the left.
Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return vbox({
               text(name),
               separator(),
               component->Render() | xflex,
           });
  });
}
 

int main(int argc, const char* argv[]) {

    auto screen = ScreenInteractive::TerminalOutput();
    char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    //std::cout << homedir << std::endl;

    std::vector<std::string> tab_values{
      "Input Folder",
      "Output Folder",
      "Select PDF",
    };
    int tab_selected = 0;
    auto tab_toggle = Toggle(&tab_values, &tab_selected);

  std::string inputFolder =  homedir;

  std::vector<std::string> pdfList;
  update_pdfList(inputFolder, &pdfList);
  auto pdfContainer = Container::Vertical({});
  bool states[50];
  for (int i = 0; i < pdfList.size(); ++i) {
    states[i] = false;
    pdfContainer->Add(Checkbox(pdfList.at(i), &states[i]));
  }
 


    std::vector<std::string> inputEntries;
    update_List(homedir, &inputEntries);
    int inputSelected = 0;
    
    MenuOption inputOption;
    inputOption.on_enter = [&] {inputFolder =  inputFolder+"/"+inputEntries.at(inputSelected); update_List(inputFolder, &inputEntries);inputSelected = 0;
        update_pdfList(inputFolder, &pdfList);
        pdfContainer->DetachAllChildren();
        //delete states;
        //bool states[pdfList.size()];
        for (int i = 0; i < pdfList.size(); ++i) {
            states[i] = false;
            pdfContainer->Add(Checkbox(pdfList.at(i), &states[i]));
        }};


    auto inputMenu = Menu(&inputEntries, &inputSelected, &inputOption);
    auto inputRender =Renderer(inputMenu, [&] {
        return vbox({
                text("Input folder => " + inputFolder+"/"+inputEntries.at(inputSelected)),
                separator(),
                inputMenu->Render() | xflex,
            });
    });


    std::vector<std::string> outputEntries;
    update_List(homedir, &outputEntries);
    int selected = 0;
    std::string outputFolder =  homedir;
    MenuOption outputOption;
    outputOption.on_enter = [&] {outputFolder =  outputFolder+"/"+outputEntries.at(selected); update_List(outputFolder, &outputEntries);selected = 0;};


    auto outputMenu = Menu(&outputEntries, &selected, &outputOption);
    auto outputRender =Renderer(outputMenu, [&] {
        return vbox({
                text("output folder => " + outputFolder+"/"+outputEntries.at(selected)),
                separator(),
                outputMenu->Render() | xflex,
            });
    });

   auto pdfCheck  = Renderer(pdfContainer, [&] {
    return vbox({pdfContainer->Render() | vscroll_indicator | size(WIDTH, GREATER_THAN, 40) | frame });
  });

    auto tab_container = Container::Tab(
    {
        inputRender,
        outputRender,
        pdfCheck,
    },
    &tab_selected);


   auto buttons = Container::Horizontal({
      Button(
          "Reset", [&] {inputFolder =  homedir; outputFolder =  homedir; update_List(outputFolder, &outputEntries); update_List(inputFolder, &inputEntries);selected = 0; inputSelected = 0; }),
    Button( 
          "Get XML", [&] { 
            exec("clear");
            std::string cmd = "./Article_handler -i " + inputFolder;
            if(!states[0]){
              cmd += " -p";
              for(int j =1; j < pdfList.size(); j++){
                if(states[j]){
                  cmd += " " + pdfList.at(j);
                }
              }
            }
            if(outputFolder != homedir){
                cmd += " -o " + outputFolder;
            } 
            cmd += " -x";
            
            exec(cmd.data());}),
    Button( 
          "Get txt", [&] {
            exec("clear");
            std::string cmd = "./Article_handler -i " + inputFolder;
            if(!states[0]){
              cmd += " -p";
              for(int j =1; j < pdfList.size(); j++){
                if(states[j]){
                  cmd += " " + pdfList.at(j);
                }
              }
            }
            if(outputFolder != homedir){
                cmd += " -o " + outputFolder;
            } 
            cmd += " -t";
            
           exec(cmd.data()); 
          }),
      Button(
          "Exit",  screen.ExitLoopClosure()),
  });

  auto layout = Container::Vertical({
      tab_toggle,
      tab_container,
      buttons,
  });
  // Display together the menu with a border
  auto renderer = Renderer(layout, [&] {
    return vbox({
                tab_toggle->Render(),
                separator(),
                tab_container->Render(),
                separator(),
                buttons->Render() | frame,
            }) | xflex | borderHeavy;
  });

  screen.Loop(renderer);

  std::cout << "Directory = " << inputFolder << std::endl;
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
