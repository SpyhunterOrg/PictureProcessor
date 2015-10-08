import QtQuick 2.4
import com.mycompany.Test 1.0

Rectangle {
    property alias mouseArea: mouseArea

    width: 600
    height: 600

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Text {
        anchors.centerIn: parent
        text: test.author
    }

    Test{
    id:test
    }


//    Rectangle
//    {
//        anchors.centerIn: parent
//        width: parent.width/2
 //       height: parent.height
  //      color: "yellow"
   // }
}
