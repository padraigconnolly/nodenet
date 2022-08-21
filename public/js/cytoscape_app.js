var cy = cytoscape({
    container: document.getElementById('cy'),
    elements: [
        { data: { id: 'a', name: 'Test1' } },
        { data: { id: 'b', name: 'Test2' } },
        {
            data: {
                id: 'ab',
                source: 'a',
                target: 'b'
            }
        }
    ],

    style: [{
        selector: 'node[name]',
        style: {
            'height': 50,
            'width': 50,
            'background-width': '75%',
            'background-height': '75%',
            'border-color': '#000',
            'border-width': 3,
            'border-opacity': 0.5,
            'content': 'data(name)'
        }
    }]
});

// Desktop Activity

function onDragStart(event) {
    event
        .dataTransfer
        .setData('text/plain', event.target.id);
}

function onDragOver(event) {
    event.preventDefault();
}

function onDrop(event) {
    const id = event
        .dataTransfer
        .getData('text');

    cy.add({
        data: { id: 'x', name: id },
        renderedPosition: { x: event.clientX, y: event.clientY },
    });
    cy.nodes('[id = "x"]').style('background-image', './images/dpdk_logo.svg');
    var data = { type: id, position: { x_cord: event.clientX, y_cord: event.clientY } };
    socket.emit('event', data);
    event
        .dataTransfer
        .clearData();
}

// Touch Activity
var clone;

function onTouchStart(e) {
    var touchLocation = e.targetTouches[0];
    console.log(e);
    var element = document.getElementById(e.currentTarget.id);
    clone = element.cloneNode(true);
    clone.style.position = 'fixed';
    clone.style.filter = 'opacity(40%)';
    l2fwd.parentNode.appendChild(clone);
    clone.style.left = (touchLocation.pageX - (clone.offsetWidth / 2)) + 'px';
    clone.style.top = (touchLocation.pageY - (clone.offsetHeight / 2)) + 'px';
}

function onTouchMove(e) {
    var touchLocation = e.targetTouches[0];
    clone.style.left = (touchLocation.pageX - (clone.offsetWidth / 2)) + 'px';
    clone.style.top = (touchLocation.pageY - (clone.offsetHeight / 2)) + 'px';
}

function onTouchEnd(e) {
    cy.add({ data: { id: 'x', name: e.currentTarget.id }, renderedPosition: { x: e.changedTouches[0].pageX, y: e.changedTouches[0].pageY } });
    cy.nodes('[id = "x"]').style('background-image', './images/dpdk_logo.svg');
    clone.remove();
}



/*window.onload = function() {
    var clone;
    // find the element that you want to drag.
    var l2fwd = document.getElementById('l2fwd');

    l2fwd.addEventListener('touchstart', function(e) {
        
    })

    l2fwd.addEventListener('touchmove', function(e) {
        var touchLocation = e.targetTouches[0];
        clone.style.left = (touchLocation.pageX - (clone.offsetWidth / 2)) + 'px';
        clone.style.top = (touchLocation.pageY - (clone.offsetHeight / 2)) + 'px';
    })

    l2fwd.addEventListener('touchend', function(e) {
        cy.add({ data: { id: 'x', name: 'Test3' }, renderedPosition: { x: e.changedTouches[0].pageX, y: e.changedTouches[0].pageY } });
        cy.nodes('[id = "x"]').style('background-image', './images/dpdk_logo.svg');
        clone.remove();
    })
}*/