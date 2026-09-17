
#include"non_rolling_log.h"
#include <string>
#include <vector>

#include <windows.h>
#include <map>


using namespace std;

struct log_position;
struct page_unit;

//This map linking the page with the function name.
map<string, page_unit> page_map;
using ite_type = map<string, page_unit>::iterator;

//Log position on console.
struct log_position
{
    //set initial position to (0,0)
    int col{0};
    int row{0};

    // Check if this is the first time a MESSAGE is printed.
    bool new_born() {
        return col == 0 && row == 0;
    }

    //set an index to indicate the place in which the content is printed to
    int place_id = 0;
    //number of printing places in a line
    int place_num = 3;
    //length of printing places
    int place_length = 36;
    //maximum print times of one message
    int place_id_max = 255;

    void next_place() {
        place_id++;
        //ensure that print times of one message does not exceed this maximum
        place_id %= place_id_max;
    }

    int place_col() {
        //ensure the place numbers in one line is fixed to "place_num"
        auto pos_id = place_id % place_num;
        //add a constant 8 here to preserve spaces for line number at the front of each line
        return  pos_id * place_length + 8;
    }
};

//With respect to the line a function is in, determine the position a message should be printed to.
using code_lines = map<int, log_position>; 

struct page_unit {
    int ori_col{ 0 }, ori_row{ 0 };
    code_lines _lines;

    // Check if this is the first time a FUNCTION is printed.
    bool new_born() {
        return ori_col == 0 && ori_row == 0;
    }
};

//Print content to assigned coordinates.
void CoordPrint(int row, int col, const char* input, int place_id = -1)
{
    if (place_id != -1) {
        printf("\033[%d;%dH %d:%s", row, col, place_id, input);
    }
    else {
        printf("\033[%d;%dH %s", row, col, input);
    }
}

void print_message(const char* fun_name, int line, const char* format, ...) {

    char message[0x100] = { 0 };

    //define parameter pointer to acquire optional parameters
    va_list param_ptr;
    //initialise parameter pointer and point ap to the address of the first actual parameter
    va_start(param_ptr, format);     

    //vsprintf(message, format, param_ptr);
    vsprintf_s(message, format, param_ptr);

    //Once a parameter pointer is no longer in use, or when the parameter pointer needs to be reinitialized, the va_end macro must be called first.
    va_end(param_ptr);


    //regenerate overlapped log serial numbers
    auto reprint_serial = [](int line_key, log_position& l_p, int page_row) {

        //Use buffer to cover the former printed line number and other contents.
        const int prn_len = 90;
        char prn_buffer[prn_len] = "                                                                                         ";
        auto str_line = std::to_string(line_key);
        memcpy(prn_buffer, str_line.c_str(), str_line.size());

        CoordPrint(page_row + l_p.row, 0, prn_buffer);
        };

    //regenerate overlapped log page numbers
    auto reprint_page = [](ite_type ipage) {

        //Use buffer to cover the former printed contents.
        const int prn_len = 120;
        char prn_buffer[prn_len] = "                                                                                                                       ";
        memcpy(prn_buffer, ipage->first.c_str(), ipage->first.size());

        CoordPrint(ipage->second.ori_row, 0, prn_buffer);
        };
     
    //regenerate overlapped log contents
    auto reprint_content = [](int row, int col, int place_id, string message) {

        //Use buffer to cover the former printed contents.
        const int prn_len = 34;
        char prn_buffer[prn_len] = "                                ";
        memcpy(prn_buffer, message.c_str(), min(message.size(), prn_len));
        
        prn_buffer[prn_len-1] = '\0';
        CoordPrint(row, col, prn_buffer, place_id);
        };


    //generate pages
    auto& page = page_map[fun_name];

    //use an iterator to locate the current (new-born) page 
    auto inew = page_map.find(fun_name);

    /*new page created*/

    if (page.new_born()) {
        //exception, when this new-born page is the first existing page.
        if (page_map.size() == 1) {
            //first row on console
            page.ori_row = 1;
            CoordPrint(page.ori_row, page.ori_col , fun_name);
        }

        else {
            //use an iterator to get row-cooradinate of the new-born page 
            auto ipage = page_map.begin();

            if (ipage == inew) {
                //initialize the ori_row to 1
                ipage->second.ori_row = 1;

                //print new-born page
                reprint_page(ipage);

                ipage++;
            }

            else {
                //initialize the ori_row to 1
                page.ori_row = 1;

                while (ipage != inew) {
                    //add an extra line for each page iterated for the page name line
                    page.ori_row += ipage->second._lines.size() + 1;
                    ipage++;
                }

                if (ipage == inew) {
                    //print new-born page
                    reprint_page(ipage);

                    ipage++;
                }
            }

            //reprint pages below new-born line.
            while (ipage != page_map.end()) {

                //print pages below new-born page
                ipage->second.ori_row += 1;

                reprint_page(ipage);
                ipage++;
            }
        } 
    }
    
    //generate lines
    auto& lines = page._lines[line];
    
    /*new line created*/

    if (lines.new_born()){
    
        //use iterator to locate the new-born line.
        auto ilines = page._lines.find(line);
        ilines->second.row = page._lines.size();
        reprint_serial(ilines->first, ilines->second, page.ori_row);
        ilines++;

        //reprint lines below current new-born line IN ITS PAGE
        while (ilines != page._lines.end()) {
            reprint_serial(ilines->first, ilines->second, page.ori_row);
            ilines++; 
        }

        //reprint lines below current new-born line IN PAGES BELOW
        inew++;

        for (; inew != page_map.end(); inew++) {
            //reprint pages below new-born line.
            inew->second.ori_row += 1;

            reprint_page(inew);

            auto& lines = inew->second._lines;
            for (auto& lu : lines) {
                //reprint line number
                reprint_serial(lu.first, lu.second, inew->second.ori_row);
            }
        }
    }

    /*update existing lines*/
    //normal print: update message

    //Get row number from specific page unit
    int msg_row = page.ori_row + lines.row;
    int msg_col = page.ori_col + lines.place_col();

    //use "place_id" to locate the print place of content
    reprint_content(msg_row, msg_col, lines.place_id, message);

    //iterate through place_id
    lines.next_place();
}

