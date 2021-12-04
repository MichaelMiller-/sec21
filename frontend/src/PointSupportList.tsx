import {useState} from "react";
import {Table} from "react-bootstrap";
import {DbPointSupport} from "./DatabaseInterface";
import {ListProps} from "./ListProps";
import {ListItemProps} from "./ListItemProps";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import NewButton from "./ui/NewButton";
import DialogAddPointSupport from "./DialogAddPointSupport";

const PointSupportItem = (props: ListItemProps<DbPointSupport>) => {
    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.type}</td>
            <td>{props.item.referencePoint.name}</td>
            <td>{props.item.ux}</td>
            <td>{props.item.uy}</td>
            <td>{props.item.uz}</td>
            <td><EditButton onClick={() => {
            }} disabled={true}/></td>
            <td><RemoveButton onClick={() => {
                props.onDelete(props.item.id)
            }} disabled={false}/></td>
        </tr>
    )
}

const PointSupportList = (props: ListProps<DbPointSupport>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false} title={"Point Support"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>Type</th>
                    <th>Reference Point</th>
                    <th>ux</th>
                    <th>uy</th>
                    <th>uz</th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<PointSupportItem onDelete={props.onDelete} item={e} />))}
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

export default PointSupportList;