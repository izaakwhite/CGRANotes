#include "syntax.hh"

#include <limits>
#include <unordered_map>
#include <unordered_set>

namespace kratos {

static std::unordered_set<std::string> system_verilog_keywords;

void inline initialize_keywords() {
    if (system_verilog_keywords.empty()) {
        auto list = {"accept_on",
                     "alias",
                     "always",
                     "always_comb",
                     "always_ff",
                     "always_latch",
                     "and",
                     "assert",
                     "assign",
                     "assume",
                     "automatic",
                     "before",
                     "begin",
                     "bind",
                     "bins",
                     "binsof",
                     "bit",
                     "break",
                     "buf",
                     "bufif0",
                     "bufif1",
                     "byte",
                     "case",
                     "casex",
                     "casez",
                     "cell",
                     "chandle",
                     "checker",
                     "class",
                     "clocking",
                     "cmos",
                     "config",
                     "const",
                     "constraint",
                     "context",
                     "continue",
                     "cover",
                     "covergroup",
                     "coverpoint",
                     "cross",
                     "deassign",
                     "default",
                     "defparam",
                     "design",
                     "disable",
                     "dist",
                     "do",
                     "edge",
                     "else",
                     "end",
                     "endcase",
                     "endchecker",
                     "endclass",
                     "endclocking",
                     "endconfig",
                     "endfunction",
                     "endgenerate",
                     "endgroup",
                     "endinterface",
                     "endmodule",
                     "endpackage",
                     "endprimitive",
                     "endprogram",
                     "endproperty",
                     "endspecify",
                     "endsequence",
                     "endtable",
                     "endtask",
                     "enum",
                     "event",
                     "eventually",
                     "expect",
                     "export",
                     "extends",
                     "extern",
                     "final",
                     "first_match",
                     "for",
                     "force",
                     "foreach",
                     "forever",
                     "fork",
                     "forkjoin",
                     "function",
                     "generate",
                     "genvar",
                     "global",
                     "highz0",
                     "highz1",
                     "if",
                     "iff",
                     "ifnone",
                     "ignore_bins",
                     "illegal_bins",
                     "implements",
                     "implies",
                     "import",
                     "incdir",
                     "include",
                     "initial",
                     "inout",
                     "input",
                     "inside",
                     "instance",
                     "int",
                     "integer",
                     "interconnect",
                     "interface",
                     "intersect",
                     "join",
                     "join_any",
                     "join_none",
                     "large",
                     "let",
                     "liblist",
                     "library",
                     "local",
                     "localparam",
                     "logic",
                     "longint",
                     "macromodule",
                     "matches",
                     "medium",
                     "modport",
                     "module",
                     "nand",
                     "negedge",
                     "nettype",
                     "new",
                     "nexttime",
                     "nmos",
                     "nor",
                     "noshowcancelled",
                     "not",
                     "notif0",
                     "notif1",
                     "null",
                     "or",
                     "output",
                     "package",
                     "packed",
                     "parameter",
                     "pmos",
                     "posedge",
                     "primitive",
                     "priority",
                     "program",
                     "property",
                     "protected",
                     "pull0",
                     "pull1",
                     "pulldown",
                     "pullup",
                     "pulsestyle_ondetect",
                     "pulsestyle_onevent",
                     "pure",
                     "rand",
                     "randc",
                     "randcase",
                     "randsequence",
                     "rcmos",
                     "real",
                     "realtime",
                     "ref",
                     "reg",
                     "reject_on",
                     "release",
                     "repeat",
                     "restrict",
                     "return",
                     "rnmos",
                     "rpmos",
                     "rtran",
                     "rtranif0",
                     "rtranif1",
                     "s_always",
                     "s_eventually",
                     "s_nexttime",
                     "s_until",
                     "s_until_with",
                     "scalared",
                     "sequence",
                     "shortint",
                     "shortreal",
                     "showcancelled",
                     "signed",
                     "small",
                     "soft",
                     "solve",
                     "specify",
                     "specparam",
                     "static",
                     "string",
                     "strong",
                     "strong0",
                     "strong1",
                     "struct",
                     "super",
                     "supply0",
                     "supply1",
                     "sync_accept_on",
                     "sync_reject_on",
                     "table",
                     "tagged",
                     "task",
                     "this",
                     "throughout",
                     "time",
                     "timeprecision",
                     "timeunit",
                     "tran",
                     "tranif0",
                     "tranif1",
                     "tri",
                     "tri0",
                     "tri1",
                     "triand",
                     "trior",
                     "trireg",
                     "type",
                     "typedef",
                     "union",
                     "unique",
                     "unique0",
                     "unsigned",
                     "until",
                     "until_with",
                     "untyped",
                     "use",
                     "uwire",
                     "var",
                     "vectored",
                     "virtual",
                     "void",
                     "wait",
                     "wait_order",
                     "wand",
                     "weak",
                     "weak0",
                     "weak1",
                     "while",
                     "wildcard",
                     "wire",
                     "with",
                     "within",
                     "wor",
                     "xnor",
                     "xor"};
        system_verilog_keywords.reserve(list.size());
        for (auto const &keyword : list) {
            system_verilog_keywords.emplace(keyword);
        }
    }
}

bool is_valid_variable_name(const std::string &name) {
    initialize_keywords();
    return system_verilog_keywords.find(name) == system_verilog_keywords.end();
}

static std::unordered_map<std::string, BuiltinFunctionInfo> builtin_functions_info = {
    {"clog2", {32, true}},
    {"countones", {32, true}},
    {"onehot", {1, true}},
    {"onehot0", {1, true}},
    {"isunknown", {1, true}},
    {"display", {0, false, 1, std::numeric_limits<uint32_t>::max()}},
    {"hgdb_assert_fail", {0, false, 3, 4}},
    {"finish", {0, false, 0, 1}},
    {"fopen", {32, false, 2, 2}},
    {"fclose", {0, false}},
    {"fscanf", {32, false, 2, std::numeric_limits<uint32_t>::max()}},
    {"urandom", {32, false, 0, 1}},
    {"random", {32, false, 0, 1, true}}};

std::optional<BuiltinFunctionInfo> get_builtin_function_info(const std::string &name) {
    if (builtin_functions_info.find(name) != builtin_functions_info.end()) {
        return builtin_functions_info.at(name);
    } else {
        return std::nullopt;
    }
}

std::set<std::string> get_builtin_function_names() {
    std::set<std::string> res;

    for (auto const &iter : builtin_functions_info) {
        res.emplace(iter.first);
    }
    return res;
}

}  // namespace kratos