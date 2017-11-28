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
        
        static float CharWidth(int c, float size) {
            if (c >= 26 && c < 52)
                return size * 12.0f / 16.0f;
            else
                return size * 13.0f / 16.0f;
        }
        
    public:
        static void Initialize() {
        }
        
        static float GetWidth(const string msg, float size) {
            int c;
            float w = 0;
            for (int i = 0; i < msg.length(); i++) {
                c = 0;
                for (int j = 0; j < characters.length(); j++) {
                    if (msg[i] == characters[j]) {
                        c = j;
                        break;
                    }
                }
                
                w += CharWidth(c, size);
            }
            return w;
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
                q->id = indexStart + i;
                q->BufferData();

                lx += CharWidth(c, size);
            }
            
            delete q;
        }
    };
}

string Gfx::Font::characters = "ABCDEFGHIJKLMNOPRRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890 ";

#endif