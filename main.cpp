/* 
* File:   main.cpp
* Author: sk
*
* Created on December 20, 2018, 6:43 PM
*/

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <termios.h>

#define UP_ARROW     65
#define LEFT_ARROW   68
#define DOWN_ARROW   66
#define RIGHT_ARROW  67
#define DELETE       51
#define HOME         72
#define END          70

using namespace std;

vector<string> cmds;
int cmds_pos = -1;

static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void parse_cmd(string cmd)
{
    // здесь обратотка данных
}

/*
 * 
 */
int main(int argc, char** argv) 
{
    FILE *_fp = fopen("history_cmds.txt", "r");

    if( _fp != NULL)
    {
        char buffer[255];

        while(fgets(buffer, 255, (FILE*) _fp))
        {
            string _cmd = "";
            _cmd.append( buffer );
            _cmd = trim(_cmd);

            cmds.push_back(_cmd);
        }

        cmds_pos = cmds.size();

        fclose(_fp);
    }
    
    string current_cmd = "";
    int c, c2, pos_in_line = 0;
    while(true)
    {
        c = getch();
        /*printf("\n%d\n", c);
        continue;*/

        if( c == 27 && getch() == 91 )
        {
            c2 = getch();

            switch ( c2 )
            {
                case UP_ARROW:
                    //cout << "UP ARROW" << endl;

                    if( cmds.size() > 0 )
                    {
                        if( cmds_pos != -1 )
                        {
                            if( cmds_pos == cmds.size() )
                            {
                                for(unsigned int i = pos_in_line; i < current_cmd.length(); i++)
                                {
                                    putchar (current_cmd[i]);
                                }

                                for(unsigned int k = 0, len = current_cmd.length(); k < len; k++)
                                {
                                    putchar ('\b');
                                    putchar (' ');
                                    putchar ('\b');
                                }
                            }
                            else
                            {
                                for(unsigned int i = pos_in_line; i < cmds[ cmds_pos ].length(); i++)
                                {
                                    putchar (cmds[ cmds_pos ][i]);
                                }

                                for(unsigned int k = 0, len = cmds[ cmds_pos ].length(); k < len; k++)
                                {
                                    putchar ('\b');
                                    putchar (' ');
                                    putchar ('\b');
                                }
                            }


                            cmds_pos -= 1;

                            if( cmds_pos < 0 )
                            {
                                cmds_pos = 0;
                            }

                            pos_in_line = cmds[ cmds_pos ].length();

                            printf("%s", cmds[ cmds_pos ].c_str() );
                        }
                    }

                    break;

                case DOWN_ARROW:

                    if( cmds.size() > 0 )
                    {
                        if( cmds_pos != cmds.size() && cmds_pos != -1 )
                        {
                            for(unsigned int i = pos_in_line; i < cmds[ cmds_pos ].length(); i++)
                            {
                                putchar (cmds[ cmds_pos ][i]);
                            }

                            for(unsigned int k = 0, len = cmds[ cmds_pos ].length(); k < len; k++)
                            {
                                putchar ('\b');
                                putchar (' ');
                                putchar ('\b');
                            }

                            cmds_pos += 1;

                            if( cmds_pos == cmds.size() )
                            {
                                printf("%s", current_cmd.c_str() );

                                pos_in_line = current_cmd.length();
                            }
                            else
                            {
                                printf("%s", cmds[ cmds_pos ].c_str() );

                                pos_in_line = cmds[ cmds_pos ].length();
                            }
                        }
                    }

                    break;

                case LEFT_ARROW:

                    putchar ('\b');

                    if( pos_in_line -1 >= 0 )
                    {
                        pos_in_line -= 1;
                    }

                    break;

                case RIGHT_ARROW:

                    if( current_cmd.length() == 0 )
                    {
                        if( pos_in_line < cmds[ cmds_pos ].length() )
                        {
                            putchar ( cmds[ cmds_pos ][pos_in_line] );
                            pos_in_line += 1;
                        }
                    }
                    else
                    {
                        if( pos_in_line < current_cmd.length() )
                        {
                            putchar ( current_cmd[pos_in_line] );
                            pos_in_line += 1;
                        }
                    }

                    break;

                case HOME:

                    for(unsigned int k = 0, len = pos_in_line; k < len; k++)
                    {
                        putchar ('\b');
                    }

                    pos_in_line = 0;

                    break;

                case END:

                    if( current_cmd.length() == 0 )
                    {
                        for(unsigned int i = pos_in_line; i < cmds[ cmds_pos ].length(); i++)
                        {
                            putchar ( cmds[ cmds_pos ][i] );
                        }

                        pos_in_line = cmds[ cmds_pos ].length();
                    }
                    else
                    {
                        for(unsigned int i = pos_in_line; i < current_cmd.length(); i++)
                        {
                            putchar ( current_cmd[i] );
                        }

                        pos_in_line = current_cmd.length();
                    }

                    break;

                /*case DELETE:

                    printf("\n%d\n", c2);

                    break;*/
                /*default:
                    printf("\n%d\n", c2);*/
            }

            if( c2 == DELETE && getch() == 126 )
            {
                if(! ( cmds_pos == -1 || cmds_pos == cmds.size() ) )
                {
                    current_cmd = trim( cmds[ cmds_pos ] );
                }

                if( pos_in_line < current_cmd.length() )
                {
                    for(unsigned int i = pos_in_line; i < current_cmd.length(); i++)
                    {
                        putchar (current_cmd[i]);
                    }

                    //printf("\n");

                    putchar ('\b');
                    putchar (' ');
                    putchar ('\b');

                    //printf("\n");

                    //printf( "\n%d %d\n", pos_in_line, current_cmd.length() );

                    current_cmd.erase(pos_in_line, 1);

                    //pos_in_line -= 1;

                    //printf( "\n=%s\n", current_cmd.c_str() );
                    //printf("\n%d\n", current_cmd.length() - pos_in_line - 1);

                    for(unsigned int k = 0, len = current_cmd.length() - pos_in_line; k < len; k++)
                    {
                        putchar ('\b');
                    }

                    for(unsigned int i = pos_in_line; i < current_cmd.length(); i++)
                    {
                        putchar (current_cmd[i]);
                    }

                    for(unsigned int k = 0, len = current_cmd.length() - pos_in_line; k < len; k++)
                    {
                        putchar ('\b');
                    }

                    //printf( "\n%s\n", current_cmd.c_str() );

                    cmds_pos = cmds.size();
                }
            }
        }
        else if( isprint(c) )
        {
            //printf("\ncmds_pos: %d %d\n", cmds_pos, cmds.size());

            if( cmds_pos == -1 || cmds_pos == cmds.size() ) // если нет пред. ввода
            {
                //printf("pos_in_line:%d current_cmd.length() - 1:%d\n", pos_in_line, current_cmd.length() );

                if( pos_in_line == current_cmd.length() )
                {
                    current_cmd += (char)c;
                    putchar (c);
                }
                else
                {
                    current_cmd.insert(pos_in_line, 1, c);

                    int diff1 = current_cmd.length() - pos_in_line;

                    //printf("%d\n", diff1);

                    putchar (c);
                    for(unsigned int k = pos_in_line + 1, len = current_cmd.length(); k < len; k++)
                    {
                        putchar (current_cmd[k]);
                    }

                    for(unsigned int k = 0, len = current_cmd.length() - pos_in_line - 1; k < len; k++)
                    {
                        putchar ('\b');
                    }
                }
            }
            else
            {
                current_cmd = trim( cmds[ cmds_pos ] );

                //pos_in_line = cmds[ cmds_pos ].length() - 1;
                //current_cmd += (char)c;

                if( pos_in_line == current_cmd.length() )
                {
                    current_cmd += (char)c;
                    putchar (c);
                }
                else
                {
                    current_cmd.insert(pos_in_line, 1, c);

                    //int diff1 = current_cmd.length() - pos_in_line;

                    //printf("\n%d\n", diff1);

                    putchar (c);
                    for(unsigned int k = pos_in_line + 1, len = current_cmd.length(); k < len; k++)
                    {
                        putchar (current_cmd[k]);
                    }

                    for(unsigned int k = 0, len = current_cmd.length() - pos_in_line - 1; k < len; k++)
                    {
                        putchar ('\b');
                    }
                }

                cmds_pos = cmds.size();
            }

            pos_in_line += 1;
        }
        else if( c == 127 )
        {
            if(! ( cmds_pos == -1 || cmds_pos == cmds.size() ) )
            {
                current_cmd = trim( cmds[ cmds_pos ] );
                cmds_pos = cmds.size();
            }

            //printf("\n%d %d\n", pos_in_line, current_cmd.length() - 1);

            if( pos_in_line == current_cmd.length() )
            {
                putchar ('\b');
                putchar (' ');
                putchar ('\b');

                /*if( cmds_pos == -1 || cmds_pos == cmds.size() )
                {
                    current_cmd = current_cmd.substr (0, current_cmd.length() );

                    pos_in_line = current_cmd.length();
                }
                else*/
                {
                    //current_cmd = trim( cmds[ cmds_pos ] );
                    current_cmd = current_cmd.substr (0, current_cmd.length() - 1 );

                    pos_in_line = current_cmd.length();

                    cmds_pos = cmds.size();
                }
            }
            else
            {

                for(unsigned int i = pos_in_line; i < current_cmd.length(); i++)
                {
                    putchar (current_cmd[i]);
                }

                //printf("\n");

                putchar ('\b');
                putchar (' ');
                putchar ('\b');

                //printf("\n");

                //printf( "\n%d %d\n", pos_in_line, current_cmd.length() );

                current_cmd.erase(pos_in_line - 1, 1);

                pos_in_line -= 1;

                //printf( "\n=%s\n", current_cmd.c_str() );
                //printf("\n%d\n", current_cmd.length() - pos_in_line - 1);

                for(unsigned int k = 0, len = current_cmd.length() - pos_in_line; k < len; k++)
                {
                    putchar ('\b');
                }

                for(unsigned int i = pos_in_line; i < current_cmd.length(); i++)
                {
                    putchar (current_cmd[i]);
                }

                for(unsigned int k = 0, len = current_cmd.length() - pos_in_line; k < len; k++)
                {
                    putchar ('\b');
                }

                //printf( "\n%s\n", current_cmd.c_str() );

                cmds_pos = cmds.size();
            }
        }
        else if( c == 10 )
        {
            putchar ('\n');

            if( cmds_pos == -1 || cmds_pos == cmds.size() )
            {
                current_cmd = trim(current_cmd);

                if( current_cmd.length() > 0 )
                {
                    if( cmds.size() == 0 || ( cmds[ cmds.size() - 1 ].compare(current_cmd) != 0 ) )
                    {
                        cmds.push_back( current_cmd );

                        FILE *fp = fopen("history_cmds.txt", "a+");

                        if (fp != NULL)
                        {
                            fprintf(fp, "%s\n", current_cmd.c_str());
                            fclose(fp);
                        }
                        else
                        {
                            printf("error write file\n");
                        }
                    }

                    //printf("_%s_\n", current_cmd.c_str());

                    parse_cmd(current_cmd);
                }
            }
            else
            {
                string f = trim( cmds[ cmds_pos ] );

                if( f.length() > 0 )
                {
                    if( cmds.size() == 0 || ( cmds[ cmds.size() - 1 ].compare(f) != 0 ) )
                    {
                        cmds.push_back( f );

                        FILE *fp = fopen("history_cmds.txt", "a+");

                        if (fp != NULL)
                        {
                            fprintf(fp, "%s\n", f.c_str());
                            fclose(fp);
                        }
                        else
                        {
                            printf("error write file\n");
                        }
                    }

                    //printf("2_%s_\n", f.c_str());

                    parse_cmd(f);
                }
            }

            if( cmds.size() > 0 )
            {
                cmds_pos = cmds.size();
            }

            current_cmd = "";
            pos_in_line = 0;
        }
        else
        {
            //printf("_%d_", c);
        }
    }
    
    return 0;
}
