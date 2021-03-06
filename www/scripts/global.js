function getHttp()
{
	var xmlhttp;
	if (window.XMLHttpRequest) // code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp=new XMLHttpRequest();
	else // code for IE6, IE5
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");

	return xmlhttp;
}

function addEvent(event)
{
	var oldLoad = document.onload;
	if (!oldLoad) {
		document.onload = event();	
	}
	else {
		document.onload = function() {
			oldLoad();
			event();
		}
	}
}

/*
 设置右键菜单
 divId -- 弹出的id
 createFun -- 创建函数
 removeFun -- 删除函数
*/
function setDivRightMenu(divId, createFun, removeFun)
{
	if (!document.getElementById) 
		return false;

	var tarEle = document.getElementById(divId);
	if (!tarEle)
		return false;

	tarEle.onmousedown = function(aEvent) {
		if (window.event)
			aEvent = window.event;

		if (aEvent.button == 2) {
			tarEle.oncontextmenu = function() {
				return false;
			};

			crcreateFun(aEvent);
		}
		else if (aEvent.button == 1) {
			removeFun();
		}
	};

	return true;
}

function createFunTest(aEvent)
{
	var menuElement = document.createElement("div");
	if (!menuElement)
		return false;

	menuElement.setAttribute("id", "div_rightMenu");
	menuElement.setAttribute("class", "div_rightMenu");

	var subMenu = document.createElement("div");
	if (!subMenu)
		return false;
	subMenu.onclick = function() {
		alert("test aaa");
		return ture;
	};

	subMenu.innerHTML = "test";

	menuElement.appendChild(subMenu);

	document.body.appendChild(menuElement);

	//set menu position
	menuElement.style.left = aEvent.clentX - 2;
	menuElement.style.top = aEvent.clentY - 2;
	menuElement.style.display = 'inline';

	return true;
}

function removeFunTest()
{
	if (!document.getElementById) 
		return false;

	var element = document.getElementById("div_rightMenu");
	if (!element)
		return false;

	element.parendNode.removeChild(element);

	return true;
}

function selFunc(source)
{
	var title = source.getAttribute("title");
	var target = document.getElementById("opraption");
	target.firstChild.nodeValue = title;	
}