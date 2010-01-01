
#include "hi.hpp"

int main( int argc, char * argv[] ) {
    char description[10000];
    char color_str[10];
    strcat(description,
"usage:\n"
"   hi [options] REGEX FOREGROUND_COLOR BACKGROUND_COLOR\\\n"
"       [ REGEX FOREGROUND_COLOR BACKGROUND_COLOR ...]\n"
"\n"
"description:\n"
"   'hi' highlights text from standard in according to the supplied regular\n"
"   expressions and color arguments.  The regular expressions are Perl style.\n"
"\n"
"   e.g.\n"
"   % echo hello mother, hello father | hi 'hel*o' RED NONE\n"
"   ");
    set_colors_str(RED,NONE,color_str);
    strcat(description, color_str);
    strcat(description,"hello");
    set_colors_str(NONE,NONE,color_str);
    
    strcat(description, color_str);
    strcat(description," mother, ");
    
    set_colors_str(RED,NONE,color_str);
    strcat(description, color_str);
    strcat(description,"hello");
    set_colors_str(NONE,NONE,color_str);
    
    strcat(description, color_str);
    strcat(description," father \n");
    strcat(description,
"\n"
"   NOTE: To view these colors in 'less' add this line to your ~/.aliases file\n"
"       setenv LESS \"$LESS -R\"\n"
"\n"
"   The available foreground and background colors are:\n");
    strcat(description, color_swatch());
    strcat(description,
"\n"
"author:\n"
"   Ross Rogers, 2009.08.20\n"
"\n"
"options");
    
    namespace po = boost::program_options;
    po::options_description opts_desc( description );
    
    opts_desc.add_options()
        ("help", "show this help message and exit")
    ;
    
    opts_desc.add_options()
        ("dbg", "enable debug printouts")
    ;
    
    po::options_description hidden_desc( "" );
    hidden_desc.add_options()
        ("h", "")
        ("input", po::value<vector<string> >()->composing(), "" )
    ;
    
    po::options_description all_options( description );
    all_options
        .add(opts_desc)
        .add(hidden_desc)
    ;
    
    po::positional_options_description positional_opts;
    positional_opts.add("input",-1);
    
    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv)
        .options(all_options)
        .positional(positional_opts)
        .allow_unregistered()
        .run();
    
    po::store(parsed, vm );
    po::notify(vm);
    
    if (argc == 1 || vm.count("help") || vm.count("h")) {
        cout << opts_desc << "\n";
        return 0;
    }
    
    bool dbg = false;
    if (vm.count("dbg")) {
        dbg = true;
    }
    
    vector<Filter*> build_filters;
    
    if (vm.count("input")) {
        vector<string> v = vm["input"].as<vector<string> >();
        Filter *f;
        // add a default benign filter at the beginning
        f = new Filter;
        f->regex = NULL;
        f->foreground = NONE;
        f->background = NONE;
        build_filters.push_back(f);
        for(int i = 0; i < v.size(); i++) {
            //cout << "processing arg '"<<v[i]<<"'\n";
            switch ( i%3 ) {
              case 0:
                f = new Filter;
                if (dbg) {
                    cout << "hi dbg: Constructing filter with regex string'"
                        <<v[i]<<"'"<<endl;
                }
                f->regex = new boost::regex(v[i]);
                //cout << "creating a regex with string "<<v[i]<<endl;
                break;
              case 1:
                f->foreground = interpret_color(v[i].c_str());
                //cout << "setting foreground\n";
                break;
              case 2:
                f->background = interpret_color(v[i].c_str());
                //cout << "setting background\n";
                build_filters.push_back(f);
                break;
            }
        }
    }
    
    // Simplifying vector to c-style array for speed, since iterating through
    // vectors showed up near the top of a gprof profile.
    unsigned char filters_size = build_filters.size();
    Filter **filters = new Filter*[filters_size];
    copy( build_filters.begin(), build_filters.end(), filters);
    
    string input_line;
    int char_filter_indices_size = 0;
    int *char_filter_indices = NULL;
    while ( cin && !cin.eof()) {
        getline(cin,input_line,'\n');
        
        bool found_regex_match = false;
        
        if (char_filter_indices_size < input_line.size()) {
            char_filter_indices_size = input_line.size();
            if (char_filter_indices != NULL) free(char_filter_indices);
            char_filter_indices = new int[char_filter_indices_size];
        }
        for ( int c_i = 0; c_i < char_filter_indices_size; c_i++ ) {
            char_filter_indices[c_i] = 0;
        }
        
        for ( int f_i = 1; f_i < filters_size; f_i++ ) {
            //cout << "trying filter with index "<< f_i<<endl;
            boost::regex *re = filters[f_i]->regex;
            boost::cmatch matches;
            //if (boost::regex_search(input_line.begin(),input_line.end(),matches,re)) {
            //if (boost::regex_search(input_line.c_str(),matches,*re)) {
            const char *current_str = input_line.c_str();
            const char *beg_line_adx = current_str;
            
            int watchdog = 0;
            
            while (boost::regex_search(current_str,matches,*re)) {
                if (!found_regex_match) found_regex_match = true;
                //cout<<"setting indices to foreground color of filter @index "<<f_i<<endl;
                
                int begin_idx = (long)matches[0].first-(long)beg_line_adx;
                int end_idx = (long)matches[0].second-(long)beg_line_adx;
                for ( int c_i = begin_idx; c_i < end_idx ; c_i++ ) {
                    char_filter_indices[c_i] = f_i;
                    //cout << " "<<c_i;
                }
                //cout <<endl;
                
                //cout << "found match for regex at index "<<f_i<<endl;
                //for ( int m_i = 0; m_i < matches.size(); m_i++ ) {
                //    cout << " beg_line_adx "<< (long)beg_line_adx << endl;
                //    cout << " current_str "<< long(current_str) << endl;
                //    cout << " current_str "<< current_str << endl;
                //    cout << " matches["<<m_i<<"].first="<<(long)matches[m_i].first<<endl;
                //    cout << " matches["<<m_i<<"].second="<<(long)matches[m_i].second<<endl;
                //    cout << " matches["<<m_i<<"].matched="<<matches[m_i].matched<<endl;
                //}
                
                if (current_str == matches[0].second ) {
                    // found a zero width match, advancing 1 char.
                    current_str = matches[0].second +1;
                    if ( '\0' == *current_str) {
                        break;
                    }
                } else {
                    current_str = matches[0].second;
                }
                watchdog += 1;
                if (watchdog == 0x100) {
                    cerr << "hi ERROR: looped "<<watchdog<<" times on regex filter number "<< f_i-1<<endl;
                    return -1;
                }
            }
        }
        
        //cerr << "here\n";
        if (found_regex_match) {
            //cerr << "here\n";
            const char *c = input_line.c_str();
            int char_index = 0;
            Filter *cur_filter = NULL;
            int cur_filter_index = 0;
            while ( *c ) {
                if ( cur_filter_index != char_filter_indices[char_index] ) {
                    cur_filter_index = char_filter_indices[char_index];
                    cur_filter = filters[cur_filter_index];
                    set_console_colors(cur_filter->foreground,cur_filter->background);
                }
                putchar( *c);
                c += 1;
                char_index += 1;
            }
            reset_console_colors();
            puts("");
        } else {
            //cerr << "there\n";
            puts(input_line.c_str());
        }
        
        //cout << input_line << endl;
    }
    //printf("Hello world!\n");
}

static color_t interpret_color( const char *color_str ) {
    if (strcmp("BLACK", color_str) == 0) return BLACK;
    if (strcmp("RED",   color_str) == 0) return RED;
    if (strcmp("GREEN", color_str) == 0) return GREEN;
    if (strcmp("BROWN", color_str) == 0) return BROWN;
    if (strcmp("BLUE",  color_str) == 0) return BLUE;
    if (strcmp("PURPLE",color_str) == 0) return PURPLE;
    if (strcmp("CYAN",  color_str) == 0) return CYAN;
    if (strcmp("WHITE", color_str) == 0) return WHITE;
    if (strcmp("NONE",  color_str) == 0) return NONE;
    //cerr << "No match for color '"<< color_str << "'\n";
    return NONE;
}

static const char * color_t_2_str( color_t c ) {
    switch ( c ) {
      case BLACK:  return "BLACK";
      case RED:    return "RED";
      case GREEN:  return "GREEN";
      case BROWN:  return "BROWN";
      case BLUE:   return "BLUE";
      case PURPLE: return "PURPLE";
      case CYAN:   return "CYAN";
      case WHITE:  return "WHITE";
      case NONE:   return "NONE";
    }
    cerr << "No match for color_t parameter c with value "<< c << endl;
    return "ERR_NO_MATCHING_COLOR_T_FOUND";
}


static const char* color_swatch() {
    string swatch = "            ";
    
    char token[20];
    // header
    for ( int fg = BLACK; fg <= NONE; ++fg ) {
        sprintf(token," %-7s ",color_t_2_str((color_t)fg));
        swatch.append(token);
    }
    swatch.append("\n   ");
    for ( int bg = BLACK; bg <= NONE; bg++ ) {
        sprintf(token," %-7s ",color_t_2_str((color_t)bg));
        swatch.append(token);
        for ( int fg = BLACK; fg <= NONE; fg++ ) {
            set_colors_str((color_t)fg,(color_t)bg,token);
            swatch.append(token);
            swatch.append(" Test    ");
            set_colors_str(NONE,NONE,token);
            swatch.append(token);
        }
        swatch.append("\n   ");
    }
    
    return swatch.c_str();
}

static void reset_console_colors() {
    static char *rst_str = NULL;
    if (NULL == rst_str) {
        rst_str = (char *)malloc(16*sizeof(char));
        set_colors_str( NONE, NONE, rst_str);
    };
    fputs(rst_str,stdout);
}

static void set_console_colors( color_t foreground, color_t background) {
    static char color_str[16]; // Color string will never be bigger than 9 chars
    set_colors_str( foreground, background, color_str);
    fputs(color_str,stdout);
}

static void set_colors_str( color_t foreground, color_t background, char *ret_color_str) {
    if ( NONE == foreground && NONE == background ) {
        sprintf(ret_color_str, "%c[0m", LNX_ESC_CHAR );
    } else if ( NONE == foreground ) {
        sprintf(ret_color_str, "%c[1;%dm", LNX_ESC_CHAR, background + BG_OFFSET );
    } else if ( NONE == background ) {
        sprintf(ret_color_str, "%c[1;%dm", LNX_ESC_CHAR, foreground + FG_OFFSET );
    } else {
        sprintf(ret_color_str, "%c[1;%d;%dm", LNX_ESC_CHAR, foreground + FG_OFFSET,background + BG_OFFSET );
    }
}

