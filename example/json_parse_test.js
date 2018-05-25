//The JSON file needs to be read as a variable in the final file that will be generated in C++
$.getJSON("Test.json", function (json) {
//  $(document).ready(function() {
  // Icons used from https://github.com/danklammer/bytesize-icons
  var info = document.getElementById("info");
  var title = document.getElementById(("title"));
  title.innerHTML = "Analyzer tool";
  info.appendChild(document.createTextNode("Project " + json["project"] + " analysis results"));
  var totals = document.getElementById("totals");
  var history = json["history"];
  if (history.length > 1) {  // More than 1 version
      var prev = history[history.length - 2];
      var curr = history[history.length - 1];
      var size_diff = parseInt(curr["size"] - prev["size"]);
      var sloc_diff = parseInt(curr["sloc"] - prev["sloc"]);
      var cloc_diff = parseInt(curr["cloc"] - prev["cloc"]);
  }
  var totals_text;
  totals_text = "<tbody><tr><td>Version: </td>" + "<td>" + json["version"] + "</td></tr>";
  totals_text += "<tr><td>Total size: </td>" + "<td>" + (json["totalsize"] / 1024).toFixed(2) + " KB";
  if (size_diff) {
    if (size_diff > 0) totals_text += " (Change: +" + (size_diff / 1024).toFixed(2) + " KB)";
    else totals_text += " (Change: " + (size_diff / 1024).toFixed(2) + " KB)";
  }
    totals_text += "</td></tr><tr><td>Total source lines of code: </td>" + "<td>" + json["totalsloc"];
  if (sloc_diff) {
    if (sloc_diff > 0) totals_text += " (Change: +" + sloc_diff + " lines)";
    else totals_text += " (Change: " + sloc_diff + " lines)";
  }
  totals_text +=  "</td></tr><tr><td>Total comment lines of code: </td>" + "<td>" + json["totalcloc"];
  if (cloc_diff) {
    if (cloc_diff > 0) totals_text += " (Change: +" + cloc_diff + " lines)";
    else totals_text += " (Change: " + cloc_diff + " lines)";
  }
  totals_text += "</td></tr></tbody>";
  totals.innerHTML = totals_text;
  add_history(history);
  var breakdown = document.getElementById("breakdown");
  var breakdown_list = document.createElement("ul");
  breakdown_list.className = "list-group";
  breakdown.appendChild(breakdown_list);
  iterate(json, breakdown_list);
});

function iterate(object, list) {
  console.log(object);

  var files = object["files"];
  for (var filekey in files) {
    var file = files[filekey];
    append_file(file, list);
  }

  var subdirectories = object["subdirectories"];
  for (var dirkey in subdirectories) {
    var directory = subdirectories[dirkey];
    append_subdir(directory, list);
  }
};

function append_file(file, list) {
  console.log(file);
  var element = document.createElement("li");
  element.className = "list-group-item";
  element.innerHTML = file.filename;   //Add filename
  $(element).hover(function() {
    $(this).css('cursor','pointer');          //Set cursor to pointer on hover
  }, function() {
    $(this).css('cursor','auto');
  });
  list.appendChild(element);

  $(element).prepend("<svg id=\"i-file\" style='padding-bottom: 4px; margin-right: 8px' class='align-middle' viewBox=\"0 0 32 32\" " +
    "width=\"24\" height=\"24\" fill=\"none\" stroke=\"currentcolor\" stroke-linecap=\"round\" stroke-linejoin=\"round\" stroke-width=\"2\">\n" +
    "    <path d=\"M6 2 L6 30 26 30 26 10 18 2 Z M18 2 L18 10 26 10\" />\n" +
    "</svg>");
  var stats = document.createElement("ul");
  if (file.source_loc !== undefined) {
    stats.innerHTML = "Size: " + file.size +
      " bytes; Source LOC: " + file.source_loc +
      "; Comment LOC: " + file.comment_loc;
  } else {
    stats.innerHTML = "Size: " + file.size + " bytes";
  }
  element.appendChild(stats);

  $(stats).hide();       //Hide stats by default
  $(element).click(function() {
    $(stats).toggle(300);   //Show or hide file stats when filename is clicked
  });
};

function append_subdir(directory, list) {
  var element = document.createElement("li");
  element.className = "list-group-item";
  element.style.zIndex = 10;  // Move subdirectory element in front to avoid glitched lines
  element.innerHTML = "<strong>" + directory["directory"] + "</strong> (Size of files in this directory: " + directory["size"] + " bytes)";
  $(element).hover(function() {
    $(this).css('cursor','pointer');    //Set cursor to pointer on hover
  }, function() {
    $(this).css('cursor','auto');
  });
  list.appendChild(element);

  $(element).prepend("<svg id=\"i-folder\" style='padding-bottom: 4px; margin-right: 10px' class='align-middle' viewBox=\"0 0 32 32\" " +
    "width=\"24\" height=\"24\" fill=\"none\" stroke=\"currentcolor\" stroke-linecap=\"round\" stroke-linejoin=\"round\" stroke-width=\"2\">\n" +
    "    <path d=\"M2 26 L30 26 30 7 14 7 10 4 2 4 Z M30 12 L2 12\" />\n" +
    "</svg>");

  var sublist_div = document.createElement("div");
  var sublist = document.createElement("ul");       //New list for files and subdirs in this directory
  sublist.className = "list-group-flush";
  list.appendChild(sublist_div);
  sublist_div.appendChild(sublist);
  iterate(directory, sublist);
  $(sublist_div).children().hide();       //Hide subdirs by default
  $(element).click(function() {
    $(sublist_div).children().toggle(500); //Show or hide subdirectory contents when directory name is clicked
    if ($(element).children("#i-folder").length !== 0) {    //If a closed folder icon is found
      $(element).children("#i-folder").replaceWith("<svg id=\"i-folder-open\" style='padding-bottom: 4px; margin-right: 10px' class='align-middle' viewBox=\"0 0 32 32\" " +
        "width=\"24\" height=\"24\" fill=\"none\" stroke=\"currentcolor\" stroke-linecap=\"round\" stroke-linejoin=\"round\" stroke-width=\"2\">\n" +
        "    <path d=\"M4 28 L28 28 30 12 14 12 10 8 2 8 Z M28 12 L28 4 4 4 4 8\" />\n" +
        "</svg>");
    }
    else {
      $(element).children("#i-folder-open").replaceWith("<svg id=\"i-folder\" style='padding-bottom: 4px; margin-right: 10px' class='align-middle' viewBox=\"0 0 32 32\" " +
        "width=\"24\" height=\"24\" fill=\"none\" stroke=\"currentcolor\" stroke-linecap=\"round\" stroke-linejoin=\"round\" stroke-width=\"2\">\n" +
        "    <path d=\"M2 26 L30 26 30 7 14 7 10 4 2 4 Z M30 12 L2 12\" />\n" +
        "</svg>");
    }
  });
};

function add_history(history) {
  console.log(history);

  var info = document.getElementById("graph_info");
  var list = document.createElement("ol");

  var canvas = document.getElementById("canvas");
  canvas.width = history.length * 100;    //Each bar is 100px
  canvas.height = 250;                    //Canvas height = 250px
  var myChart = new Chart(
    {
      canvas: canvas,
      data: history,
      colors: ["#a55ca5", "#67b6c7", "#bccd7a", "#eb9743"]
    }
  );
  myChart.draw();
};

function fillBackground(ctx, width, height) {
  ctx.save();
  ctx.fillStyle = "#eeeeee";
  ctx.fillRect(0, 0, width, height);
}

function drawBar(ctx, upperLeftCornerX, upperLeftCornerY, width, height, value, version, color) {
  ctx.save();
  ctx.fillStyle = color;
  ctx.fillRect(upperLeftCornerX, upperLeftCornerY, width, height);   //Fill bar with a color

  ctx.fillStyle = "#000000";      //Add black text for size
  ctx.font = "10px Arial";
  var text = (value / 1024).toFixed(2) + " kilobytes";    //Convert value to kilobytes
  var textWidth = ctx.measureText(text).width;
  var posX = upperLeftCornerX + ((width - textWidth) / 2);    //Text X coodrdinate (centered above the bar)
  var posY = canvas.height - height - 40;  //Place the text above the bar
  ctx.textBaseline = "top";
  ctx.fillText(text, posX, posY);

  var version_width = ctx.measureText(version).width;
  posX = upperLeftCornerX + ((width - version_width) / 2);
  posY = canvas.height - 15;
  ctx.fillText(version, posX, posY);
  ctx.restore();
}

var Chart = function (options) {
  this.options = options;
  this.canvas = options.canvas;
  this.ctx = this.canvas.getContext("2d");
  this.colors = options.colors;

  this.draw = function () {
    var maxValue = 0;
    for (var element in this.options.data) {
      var curValue = parseInt(this.options.data[element].size);
      if (curValue > maxValue) {
        maxValue = curValue;
      }
    }
    var minValue = maxValue;
    for (var element in this.options.data) {
      var curValue = parseInt(this.options.data[element].size);
      if (curValue < minValue) {
        minValue = curValue;
      }
    }

    //drawing the bars
    var barIndex = 0;
    var numberOfBars = this.options.data.length;
    var barWidth = (this.canvas.width / numberOfBars);

    var bar_area_height = this.canvas.height - 20;  //20 off the bottom to add version
    var minHeight = bar_area_height * 0.1;     //Minimum height of a bar (10% of max area height)
    var maxHeight = bar_area_height - 30;     //Maximum height of a bar (-30px for size text above the bar)

    fillBackground(this.ctx, this.canvas.width, bar_area_height);     //Fill bar area with a grey background
    for (element in this.options.data) {
      var value = this.options.data[element].size;
      var version = this.options.data[element].version;

      // Height relative to max and min values
      if (minValue === maxValue) var barHeight = maxHeight;
      else var barHeight = minHeight + ((maxHeight - minHeight) * (value - minValue) / (maxValue - minValue));

      drawBar(
        this.ctx,
        barIndex * barWidth,            //Upper left corner X
        bar_area_height - barHeight, //Upper left corner Y
        barWidth,
        barHeight,
        value,
        version,
        this.colors[barIndex % this.colors.length]
      );
      barIndex++;
    }
  }
};

