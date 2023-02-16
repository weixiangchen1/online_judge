#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctemplate/template.h>

#include "./oj_model.hpp"

namespace ns_view
{
    using namespace ns_model;

    const std::string ctemplate_path = "./ctemplate_html/";
    class View
    {
    public:
        View() {}
        ~View() {}
    public:
        void AllExpandHtml(const std::vector<struct Question>& questions, std::string* html)
        {
            // 题目编号  题目标题  题目难度
            // 1. 形成路径
            std::string src_html = ctemplate_path + "all_questions.html";
            // 2. 形成字典集
            ctemplate::TemplateDictionary root("all_questions");
            for (const auto& q : questions)
            {
                ctemplate::TemplateDictionary* sub = root.AddSectionDictionary("question_list");
                sub->SetValue("number", q.number);
                sub->SetValue("title", q.title);
                sub->SetValue("level", q.level);
            }
            // 3. 获取被渲染的html
            ctemplate::Template* tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            // 4. 开始完成渲染功能
            tpl->Expand(html, &root);
        }
        void OneExpandHtml(const struct Question& q, std::string* html)
        {
            // 1. 形成路径
            std::string src_html = ctemplate_path + "one_question.html";
            // 2. 形成字典集
            ctemplate::TemplateDictionary root("one_question");
            root.SetValue("number", q.number);
            root.SetValue("desc", q.desc);
            root.SetValue("title", q.title);
            root.SetValue("level", q.level);
            root.SetValue("pre_code", q.header);
            // 3. 获取被渲染的html
            ctemplate::Template* tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            // 4. 开始完成渲染功能
            tpl->Expand(html, &root);
        }
    };
}