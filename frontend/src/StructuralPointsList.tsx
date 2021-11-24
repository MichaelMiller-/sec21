import {useState} from "react";
import {DbStructuralPoint} from "./DatabaseInterface";
import {ListProps} from "./ListProps";
import {ListItemProps} from "./ListItemProps";
import NewButton from "./ui/NewButton";
import {Table} from "react-bootstrap";
import RemoveButton from "./ui/RemoveButton";
import DialogAddStructuralPoint from "./DialogAddStructuralPoint";

const StructuralPointsListItem = (props: ListItemProps<DbStructuralPoint>) => {
    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.coordinate_x}</td>
            <td>{props.item.coordinate_y}</td>
            <td>{props.item.coordinate_z}</td>
            <td>{props.item.uuid}</td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))} /></td>
        </tr>
    )
}

const StructuralPointsList = (props: ListProps<DbStructuralPoint>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
            <NewButton onClick={() => setShowNewDialog(true)} title={"Structural Points"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>X</th>
                    <th>Y</th>
                    <th>Z</th>
                    <th>UUID</th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<StructuralPointsListItem onDelete={props.onDelete} item={e} />))}
                </tbody>
            </Table>

            <DialogAddStructuralPoint
                projectId={props.projectId}
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
                onSubmit={props.onUpdate}
            />
        </>
    )
}

export default StructuralPointsList;

