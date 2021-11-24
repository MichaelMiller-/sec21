import {useState} from "react";
import {Table} from "react-bootstrap";
import {DbSurfaceMember} from "./DatabaseInterface";
import DialogAddPointSupport from "./DialogAddPointSupport";
import {ListProps} from "./ListProps";
import {ListItemProps} from "./ListItemProps";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import NewButton from "./ui/NewButton";

const SurfaceMemberItem = (props: ListItemProps<DbSurfaceMember>) => {

    const node_names = props.item.nodes.map((e) => {
        return e.name
    })

    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.thicknessType}</td>
            <td>{props.item.thickness}</td>
            <td>{props.item.systemPlane}</td>
            <td>{props.item.coordinateX}</td>
            <td>{props.item.coordinateY}</td>
            <td>{props.item.coordinateZ}</td>
            <td>{node_names.toString()}</td>
            <td><EditButton onClick={() => {}} disabled={true}/></td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))} /></td>
        </tr>
    )
}

const SurfaceMemberList = (props: ListProps<DbSurfaceMember>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={true} title={"Surface Members"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>Thickness type</th>
                    <th>Thickness</th>
                    <th>System Plane</th>
                    <th>Coordinate X</th>
                    <th>Coordinate Y</th>
                    <th>Coordinate Z</th>
                    <th>Nodes</th>
                    <th></th>
                    <th></th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<SurfaceMemberItem onDelete={props.onDelete} item={e} />))}
                </tbody>
            </Table>

            <DialogAddPointSupport
                projectId={props.projectId}
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
                onSubmit={props.onUpdate}
            />
        </>
    )
}

export default SurfaceMemberList