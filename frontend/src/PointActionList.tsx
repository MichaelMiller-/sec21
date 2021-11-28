import {useState} from "react";
import {Table} from "react-bootstrap";
import {DbPointAction,} from "./DatabaseInterface";
import DialogAddPointAction from "./DialogAddPointAction";
import {ListItemProps} from "./ListItemProps";
import EditButton from "./ui/EditButton";
import CopyButton from "./ui/CopyButton";
import NewButton from "./ui/NewButton";
import RemoveButton from "./ui/RemoveButton";
import {ListProps} from "./ListProps";

const PointActionListItem = (props: ListItemProps<DbPointAction>) => {
    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.value_x}</td>
            <td>{props.item.value_y}</td>
            <td>{props.item.value_z}</td>
            <td>{props.item.loadcase.name}</td>
            <td>{props.item.referencePoint.name}</td>
            <td><EditButton onClick={() => {
            }} disabled={true}/></td>
            <td><CopyButton onClick={() => {
            }} disabled={true}/></td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))} /></td>
        </tr>
    )
}

const PointActionList = (props: ListProps<DbPointAction>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false} title={"Point Action"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>X [N]</th>
                    <th>Y [N]</th>
                    <th>Z [N]</th>
                    <th>Loadcase</th>
                    <th>Reference</th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<PointActionListItem onDelete={props.onDelete} item={e}/>))}
                </tbody>
            </Table>

            <DialogAddPointAction
                projectId={props.projectId}
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
                onSubmit={props.onUpdate}
            />
        </>
    )
}

export default PointActionList;