var cy = cytoscape({
    container: document.getElementById('cy'),
    /*elements: [
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
*/
/*    elements: {
        nodes: [
            { data: { id: 'j', name: 'Jerry' } },
            { data: { id: 'e', name: 'Elaine' } },
            { data: { id: 'k', name: 'Kramer' } },
            { data: { id: 'g', name: 'George' } }
        ],
        edges: [
            { data: { source: 'j', target: 'e' } },
            { data: { source: 'j', target: 'k' } },
            { data: { source: 'j', target: 'g' } },
            { data: { source: 'e', target: 'j' } },
            { data: { source: 'e', target: 'k' } },
            { data: { source: 'k', target: 'j' } },
            { data: { source: 'k', target: 'e' } },
            { data: { source: 'k', target: 'g' } },
            { data: { source: 'g', target: 'j' } }
        ]
    },*/
          

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
    },

    {
        selector: 'edge',
        style: {
          'curve-style': 'bezier',
          'target-arrow-shape': 'triangle'
        }
    },

    {
        selector: '.eh-handle',
        style: {
          'background-color': 'red',
          'width': 12,
          'height': 12,
          'shape': 'ellipse',
          'overlay-opacity': 0,
          'border-width': 12, // makes the handle easier to hit
          'border-opacity': 0
        }
      },

      {
        selector: '.eh-hover',
        style: {
          'background-color': 'red'
        }
      },

      {
        selector: '.eh-source',
        style: {
          'border-width': 2,
          'border-color': 'red'
        }
      },

      {
        selector: '.eh-target',
        style: {
          'border-width': 2,
          'border-color': 'red'
        }
      },

      {
        selector: '.eh-preview, .eh-ghost-edge',
        style: {
          'background-color': 'red',
          'line-color': 'red',
          'target-arrow-color': 'red',
          'source-arrow-color': 'red'
        }
      },

      {
        selector: '.eh-ghost-edge.eh-preview-active',
        style: {
          'opacity': 0
        }
      }
]
});

var eh = cy.edgehandles();

// Popper

// var popper;
var popperNode;
var popper;
var popperDiv;
var started = false;

function start() {
  eh.start(popperNode);
}

function stop() {
  eh.stop();
}

function setHandleOn(node) {
  if (started) { return; }

  removeHandle(); // rm old handle

  popperNode = node;

  popperDiv = document.createElement('div');
  popperDiv.classList.add('popper-handle');
  popperDiv.addEventListener('mousedown', start);
  document.body.appendChild(popperDiv);

  popper = node.popper({
    content: popperDiv,
    popper: {
      placement: 'top',
      modifiers: [
        {
          name: 'offset',
          options: {
            offset: [0, -10],
          },
        },
      ]
    }
  });
}

function removeHandle() {
  if (popper){
    popper.destroy();
    popper = null;
  }

  if (popperDiv) {
    document.body.removeChild(popperDiv);
    popperDiv = null;
  }

  popperNode = null;
}

cy.on('mouseover', 'node', function(e) {
  setHandleOn(e.target);
});

cy.on('grab', 'node', function(){
  removeHandle();
});

cy.on('tap', function(e){
  if (e.target === cy) {
    removeHandle();
  }
});

cy.on('zoom pan', function(){
  removeHandle();
});

window.addEventListener('mouseup', function(e){
  stop();
});

cy.on('ehstart', function(){
  started = true;
});

cy.on('ehstop', function(){
  started = false;
});


// Desktop Activity

function onDragStart(event) {
    event
        .dataTransfer
        .setData('text/plain', event.target.id);
        
    event
        .dataTransfer
        .setData('srcImage', event.target.children[0].currentSrc)

    console.log(event)
}

function onDragOver(event) {
    event.preventDefault();
}

function onDrop(event) {
    const id = event
        .dataTransfer
        .getData('text');
    const src = event
        .dataTransfer
        .getData('srcImage');
    console.log("src =",src)
    console.log("ID =",id)
    object = cy.add({
        data: { name: id },
        renderedPosition: { x: event.clientX, y: event.clientY },
    });
    cy.nodes(object).style('background-image', src);
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