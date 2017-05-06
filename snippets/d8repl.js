
function LineHistory() {
    this.history = []
}

LineHistory.prototype.append = function (l) {
    this.history.push(l)
}

LineHistory.prototype.toString = function () {
    ret = "["
    for (var i=0; i<this.history.length; i++) {
        if (i != 0) 
            ret += " "
        ret += this.history[i]
    }
    ret += "]"
    return ret
}

lh = new LineHistory();

for (;;) {
    var line = readline()
    if (line == "quit")
        break;
    lh.append(line)
    print (line + "\n" + lh)
}
