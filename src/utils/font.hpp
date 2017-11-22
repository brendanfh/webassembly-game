#ifndef __FONT_H_
#define __FONT_H_

#include "../gfx.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace Gfx {
    class Font {
    private:
        static string characters;
        
        static const int startx = 0, starty = 62;
    public:
        static void Initialize() {
        }
        
        static void Draw(const string msg, int indexStart, float x, float y, float size) {
            Draw(msg, indexStart, x, y, size, 1, 1, 1);     
        }
        
        static void Draw(const string msg, int indexStart, float x, float y, float size, float r, float g, float b) {
            Gfx::Quad* q = new Gfx::Quad(indexStart);
            q->SetColor(r, g, b, 1);
            
            int c;
            float lx = (float) x;
            for (int i = 0; i < msg.length(); i++) {
                //cout << "Printing " << msg[i] << " " << i << endl;
                c = 0;
                for (int j = 0; j < characters.length(); j++) {
                    if (msg[i] == characters[j]) {
                        c = j;
                        break;
                    }
                }
                
                q->SetSubTexture(startx + c % 62, starty + c / 62, 1, 1, 64, 64);
                q->SetRect(lx, y, size, size);
                if (c >= 26 && c < 52) 
                    lx += size * 13.0f / 16.0f;
                else
                    lx += size * 14.0f / 16.0f;
                q->id = indexStart + i;
                q->BufferData();
            }
            
            delete q;
        }
    };
}

string Gfx::Font::characters = "ABCDEFGHIJKLMNOPRRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890 ";

#endif