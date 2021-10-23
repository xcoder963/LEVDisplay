var lat = 20.014761684808622;
var lon = 73.76375372656331;

map = L.map('mapDiv').setView([lat, lon], 13);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors',
    maxZoom: 18,
}).addTo(map);

marker = L.marker([lat, lon]).addTo(map);