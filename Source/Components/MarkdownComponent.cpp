/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MarkdownComponent.h"
#include "cmake_versions.cpp"

MarkdownComponent::~MarkdownComponent() {

}

MarkdownComponent::MarkdownComponent(GLOBAL * global)
{
	Global = global;
	__styles[TokenType::Text] =		Style();
	__styles[TokenType::Heading] =	Style(Font(Font::getDefaultSansSerifFontName(), 30, Font::bold),Swatch::white,0.0f,8.0f,Justification::centred);
	__styles[TokenType::Heading2] = Style(Font(Font::getDefaultSansSerifFontName(), 23, Font::bold),Swatch::white,0.0f,6.0f,Justification::centred);
	__styles[TokenType::Heading3] = Style(Font(Font::getDefaultSansSerifFontName(), 20, Font::bold),Swatch::white,0.0f,4.0f,Justification::centred);
	__styles[TokenType::ListItem] = Style(Font(Font::getDefaultSansSerifFontName(), 16, Font::plain),Swatch::white,15.0f);

}

void MarkdownComponent::paint(Graphics& g) {
	Font text(Font::getDefaultSansSerifFontName(), 16, Font::plain);
	g.setColour(Swatch::white);
	g.setFont(text);
	float lineHeight = g.getCurrentFont().getHeightInPoints();
	Rectangle<int> b = getLocalBounds().reduced(8);//Padding
	Point<float> p;
	p.x = b.getX();
	p.y = b.getX();
	Env e;
	if (getNumChildComponents() != 0)
		e.renderLinks = false;
	else
		e.renderLinks = true;
	for (auto l : __lines) {
		PaintToken(p, l,e, g);
		TokenType h = l.getHeading();
		if (h == Heading || h == Heading2 || h == Heading3) {
			p.y += __styles[h].font.getHeightInPoints() + __styles[h].vPadd;
		}else
			p.y += lineHeight+2.0f;

	}
}
Point<float> MarkdownComponent::PaintToken(Point<float> p, MarkdownComponent::Token t,MarkdownComponent::Env e, Graphics &g)
{
	
	if (t.tt == Link)
	{
		Font f = __styles[Text].font;
		if (e.renderLinks) {
			
			if (e.isBold())
				f.setBold(true);
			if (e.isItalic())
				f.setItalic(true);
			HyperlinkButton* hpb = new HyperlinkButton(String(t.data), String(t.data2));
			this->__hbutts.push_back(std::unique_ptr<HyperlinkButton>(hpb));
			addAndMakeVisible(hpb);
			hpb->setColour(HyperlinkButton::ColourIds::textColourId, Swatch::white);
			hpb->setBounds(p.x - 1, p.y - 3, f.getStringWidthFloat(t.data) + 2, f.getHeightInPoints() + 8);
			f.setUnderline(true);
			hpb->setFont(f, false);
		}
		
		p.x += f.getStringWidthFloat(t.data);
	}else if (t.tt != TokenType::Text)
	{
		e.addControl(t.tt);
		for (auto token : t.SubTokens)
		{
			p = PaintToken(p, token, e, g);
		}
	}
	else 
	{
		Justification j = Justification::topLeft;
		Font f = g.getCurrentFont();
		Font back(f);
		
		if (e.getHeading() != Empty) {
			Style s = __styles[e.getHeading()];
			f = s.font;
			j = s.justification;
		}
		for (size_t i = 0; i < e.getListItem(); i++)
		{
			if (i == 0) {
				Style s = __styles[ListItem];
				f = s.font;
				j = s.justification;
			}
			float ind = __styles[ListItem].indent;
			float eSize = f.getHeight() / 3.0f;
			g.fillEllipse(p.x + ind / 2 - eSize - 2, p.y + f.getHeight() / 2 - eSize / 2, eSize, eSize);
			//Draw list stuff
			p.x += ind;
		}


		auto it = e.controllTokens.begin();
		while (it != e.controllTokens.end())
		{
			if (*it == ListItem)
			{
				it = e.controllTokens.erase(it);
			}
			else
			{
				it++;
			}
		}


		if (e.isBold())
			f.setBold(true);
		if (e.isItalic())
			f.setItalic(true);
		g.setFont(f);

		g.drawText(t.data, p.x, p.y, getLocalBounds().getWidth() - p.x, f.getHeight(), j);
		p.x += f.getStringWidthFloat(t.data);
		g.setFont(back);

	}
	return p;
}

void MarkdownComponent::resized() 
{
}

//https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string/3418285
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

void MarkdownComponent::addLine(std::string str)
{
	//if (str[str.length() - 1] == '\n')
	//	str = str.substr(0, str.length());

	if (CMAKE::VERSION_INFO.size() % 2 == 0) {
		for (size_t i = 0; i < CMAKE::VERSION_INFO.size(); i+=2)
		{
			std::string var = std::string("${") + CMAKE::VERSION_INFO[i] + std::string("}");
			replaceAll(str, var, CMAKE::VERSION_INFO[i + 1]);
		}
	}
	Token t;
	parseLine(t, str, true);
	__lines.push_back(t);

}

int MarkdownComponent::measureHeight()
{
	float height = 16.0f;

	float lineHeight = __styles[Text].font.getHeightInPoints();

	for (auto l : __lines) {
		TokenType h = l.getHeading();
		if (h == Heading || h == Heading2 || h == Heading3) {
			height += __styles[h].font.getHeightInPoints() + __styles[h].vPadd;
		}
		else
			height += lineHeight + 2.0f;

	}

	return height;
}

void MarkdownComponent::parseLine(MarkdownComponent::Token &p , std::string str, bool first)
{
	if (str == "")
		return;
	Token t;
	if (first && str.substr(0,3) == "###")
	{
		t.tt = TokenType::Heading3;
		parseLine(t, str.substr(3));
	}
	else if (first && str.substr(0, 2) == "##")
	{
		t.tt = TokenType::Heading2;
		parseLine(t, str.substr(2));
	}
	else if (first && str[0] == '#')
	{
		t.tt = TokenType::Heading;
		parseLine(t, str.substr(1));
	}
	else  if ((first||p.tt==TokenType::ListItem) && (str.substr(0, 2) == "* " || str.substr(0, 2) == "- " || str.substr(0, 2) == "+ "))
	{
		t.tt = TokenType::ListItem;
		parseLine(t, str.substr(2));
	}
	else if (str.find("**") != std::string::npos)
	{
		size_t i = str.find("**");
		std::string begin, middle, end;
		begin = str.substr(0, i);
		parseLine(p, begin);
		middle = str.substr(i+2);
		if (middle.find("**") != std::string::npos)
		{
			size_t j = middle.find("**");

			Token b;
			b.tt = TokenType::Bold;
			end = middle.substr(j + 2);
			middle = middle.substr(0, j);
			
			parseLine(b, middle);
			p.SubTokens.push_back(b);
			parseLine(p, end);

		}
		else if(middle.find("*") != std::string::npos)
		{
			size_t j = middle.find("*");
			Token star;
			star.tt = TokenType::Text;
			star.data = "*";
			p.SubTokens.push_back(star);
			parseLine(p, str.substr(i + 1));
		}
		else
		{
			Token star;
			star.tt = TokenType::Text;
			star.data = "**";
			p.SubTokens.push_back(star);
			parseLine(p, str.substr(i + 2));
		}

	}
	else  if (str.find("*") != std::string::npos)
	{
		size_t i = str.find("*");
		std::string begin, middle, end;
		begin = str.substr(0, i);
		parseLine(p, begin);
		middle = str.substr(i + 1);
		if (middle.find("*") != std::string::npos)
		{
			size_t j = middle.find("*");

			Token b;
			b.tt = TokenType::Italic;
			end = middle.substr(j + 1);
			middle = middle.substr(0, j);

			parseLine(b, middle);
			p.SubTokens.push_back(b);
			parseLine(p, end);

		}
		else
		{
			Token star;
			star.tt = TokenType::Text;
			star.data = "*";
			p.SubTokens.push_back(star);
			parseLine(p, str.substr(i + 1));
		}

	}
	else if (str.find("[") != std::string::npos)
	{
		size_t i = str.find("[");
		std::string begin, middle, end;
		begin = str.substr(0, i);
		parseLine(p, begin);
		middle = str.substr(i + 1);
		if (middle.find("]") != std::string::npos && middle[middle.find("]")+1]=='(' && middle.find(")")>middle.find("]") + 1)
		{
			size_t j = middle.find("]");

			Token b;
			b.tt = TokenType::Link;
			end = middle.substr(j + 1);
			middle = middle.substr(0, j);
			b.data = middle;

			j = end.find("(");
			middle = end.substr(j + 1, end.find(")")-(j+1));
			end = end.substr(end.find(")")+1);
			b.data2 = middle;

			p.SubTokens.push_back(b);
			parseLine(p, end);

		}
		else
		{
			Token star;
			star.tt = TokenType::Text;
			star.data = "[";
			p.SubTokens.push_back(star);
			parseLine(p, str.substr(i + 1));
		}
	}
	else
	{
		t.tt = TokenType::Text;
		t.data = str;
	}
	if (t.tt != Empty)
		p.SubTokens.push_back(t);
}


 void MarkdownComponent::parseText(MarkdownComponent::Token &t,std::string str)
{
	 if (str == "")
		 return;
	if (isToken(str[0])) 
	{
		switch (str[0])
		{
		case '#':
		{
			Token st;
			st.parent = &t;
			size_t hCount = 0;
			while (str[hCount + 1] == '#')
				hCount;
			switch (hCount)
			{
			case 0:
				st.tt = TokenType::Heading;
				break;
			case 1:
				st.tt = TokenType::Heading2;
				break;
			default:
				st.tt = TokenType::Heading3;
				break;
			}
			size_t i = hCount+1;
			while (!isToken(str[i]))
				i++;
			if (str[i] == '\n') {
				//Break heading
				st.data = str.substr(hCount+1, i-1);
				t.SubTokens.push_back(st);
				Token stt;
				stt.parent = &t;
				parseText(stt, str.substr(hCount + i+1));
				t.SubTokens.push_back(stt);
			}
			else {
				st.data = str.substr(hCount, i);
				parseText(st, str.substr(hCount + i));
				t.SubTokens.push_back(st);
			}

			break;
		}
		case '\n':
		{
			Token st;
			st.parent = &t;
			st.tt = TokenType::Newline;
			t.SubTokens.push_back(st);
			parseText(t, str.substr(1));
			break;
		}
		default:
			break;
		}

	}
	else
	{
		size_t i = 0;
		while (!isToken(str[i]) && i < str.length())
		i++;
		t.data = str.substr(0,i);
		t.tt = TokenType::Text;
		parseText(t, str.substr(i));
	}


}
bool MarkdownComponent::isToken(char c) 
{
	switch (c)
	{
	case '#':
		return true;
	case '*':
		return true;
	case '\n':
		return true;
	default:
		return false;
		break;
	}
}