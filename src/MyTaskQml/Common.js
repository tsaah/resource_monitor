function stringifyMemory(value) {
    let i = 0;
    while (value >= 1024) {
        value /= 1024;
        ++i;
    }
    let unit = [ 'B', 'KiB', 'MiB', 'GiB', 'TiB' ];
    return value.toFixed(2) + ' ' + unit[i];
}

function stringifyPercent(value, totalValue) {
    return (100 * value / totalValue).toFixed(0) + '%';
}


var firstWidth = 45
var secondWidth = 35
var rightWidth = 70
var barHeight = 10
var labelHeight = 16
var barBG = '#53585e'
var barFG = '#68a176'
var diskPaneBG = '#3C3F43'
var animationDuration = 500
