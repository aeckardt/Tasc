#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <string>
#include <vector>
#include <unordered_map>

#include <QPoint>
#include <QRect>
#include <QDateTime>
#include <QBrush>

#include "lexer.h"
#include "parser.h"
#include "parameter.h"

namespace tw
{

using ps::Node;

typedef void(* OutputFnc)(const Parameter &, const QBrush &);
typedef bool(* CommandFnc)(const ParameterList &, Parameter &);

struct Command
{
    CommandFnc callback_fnc;
    std::vector<std::vector<ParameterType>> param_types;
    ParameterType return_type;
};

class ASTWalker
{
public:
    ASTWalker() : output_fnc(nullptr) {}

    bool run(const std::string &str);

    inline void registerCommand(const std::string &name, const CommandFnc &callback_fnc,
        const std::vector<std::vector<ParameterType>> &param_types, const ParameterType &return_type)
    { commands[name] = {callback_fnc, param_types, return_type}; }

    template<class T>
    inline void registerObject(const std::string &name)
    { obj_names[ParameterObjectBase<T>::ref] = name; }

    inline void setErrorOutput(const OutputFnc &fnc)
    { output_fnc = fnc; }

private:
    OutputFnc output_fnc;

    template<class... _Args>
    void errorMsg(const char *format, _Args ... __args) const;

    std::unordered_map<std::string, Command> commands;
    std::unordered_map<ObjectReference, std::string> obj_names;

    std::unordered_map<std::string, Parameter> vars;
    Parameter return_value;

    Parameter getConstValue(const Node &node);
    bool traverse(const Node &node);
    bool traverseAssignment(const Node &node);
    bool traverseExpr(const Node &node);
    bool traverseFunction(const Node &node);
    bool traverseIfStatement(const Node &node);
    bool traverseOperation(const lx::TokenId &op, const Parameter &p1, const Parameter &p2);

    // the following types, variables and functions are exclusively used to validate the syntax
    typedef std::vector<ParameterType> ParameterTypeList;

    std::unordered_map<std::string, ParameterType> var_types;
    ParameterType return_value_type;

    Parameter::Type getParamType(const Node &node);
    bool validate(const Node &node);
    bool validateAssignment(const Node &node);
    bool validateCommand(const std::string &command, const ParameterTypeList &param_types);
    bool validateExpr(const Node &node);
    bool validateFunction(const Node &node);
    bool validateIfStatement(const Node &node);
    bool validateOperation(const lx::TokenId &op, const Parameter::Type &pt1, const Parameter::Type &pt2);
    bool validateParamType(const Node &node, ParameterTypeList *param_types = nullptr);
};

} // namespace tw

#endif // SCRIPTENGINE_H