#ifndef MARKDOWN_COMPONENT_H
#define MARKDOWN_COMPONENT_H

#include "JuceHeader.h"
#include "Global.h"
#include "Swatch.h"
#include <map>
class MarkdownComponent : public Component{

public :
	MarkdownComponent(GLOBAL * global);
	~MarkdownComponent();

	void paint(Graphics& g) override;
	void resized() override;
	void setText(std::string str);
	void addLine(std::string str);

private:
	enum TokenType {
		Text,
		Newline,
		Heading,
		Heading2,
		Heading3,
		Bold,
		Italic,
		ListItem,
		Empty
	};
	struct Token {
		std::string data;
		std::vector<Token> SubTokens;
		Token* parent=nullptr;
		TokenType tt = Empty;
		TokenType getHeading() {
			if (tt == TokenType::Heading || tt == TokenType::Heading2 || tt == TokenType::Heading3)
				return tt;

			for (auto t : SubTokens)
			{
				if (t.tt == TokenType::Heading || t.tt == TokenType::Heading2 || t.tt == TokenType::Heading3)
					return t.tt;
			}
			return Empty;
		}
	};
	struct Style {
		Font font;
		Colour colour;
		float indent;
		float vPadd;
		Justification justification;
		Style():
			font(Font::getDefaultSansSerifFontName(), 16, Font::plain),
			colour(Swatch::white),
			indent(0.0f),
			vPadd(2.0f),
			justification(Justification::topLeft)
		{

		}
		Style(Font f, Colour c, float i = 0.0f, float vp = 2.0f, Justification j = Justification::topLeft):
			font(f),
			colour(c),
			indent(i),
			vPadd(vp),
			justification(j)
		{
		}
	};
	struct Env {
		std::vector<TokenType> controllTokens;
		void addControl(TokenType t) {
			controllTokens.push_back(t);
		}
		bool isBold()
		{
			for (auto t : controllTokens)
			{
				if (t == Bold)
					return true;
			}
			return false;
		}
		bool isItalic()
		{
			for (auto t : controllTokens)
			{
				if (t == Italic)
					return true;
			}
			return false;
		}
		TokenType getHeading()
		{
			for (auto t : controllTokens)
			{
				if (t == Heading || t == Heading2 || t == Heading3)
					return t;
			}
			return Empty;
		}
		int getListItem()
		{
			int c = 0;
			for (auto t : controllTokens)
			{
				if (t == ListItem)
					c++;
			}
			return c;
		}

	};
	std::map<TokenType,Style> __styles;
	std::vector<Token> __lines;
	GLOBAL * Global;

	void parseLine(MarkdownComponent::Token &p,std::string str,bool first = false);

	void parseText(MarkdownComponent::Token &t,std::string str);
	bool isToken(char c);

	Point<float> PaintToken(Point<float>, Token, Env, Graphics &);
};



#endif //MARKDOWN_COMPONENT_H