import {useState} from "react";
import {Table} from "react-bootstrap";
import {DbMaterial} from "./DatabaseInterface";
import DialogAddMaterial from "./DialogAddMaterial";
import {ListProps} from "./ListProps";
import {ListItemProps} from "./ListItemProps";
import NewButton from "./ui/NewButton";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import CopyButton from "./ui/CopyButton";

const MaterialListItem = (props: ListItemProps<DbMaterial>) => {
    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.E_modulus}</td>
            <td><EditButton onClick={() => {}} disabled={true}/></td>
            <td><CopyButton onClick={() => {}} disabled={true}/></td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))} disabled={false}/></td>
        </tr>
    )
}

const MaterialList = (props: ListProps<DbMaterial>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false} title={"Materials"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>E modules [MPa]</th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<MaterialListItem onDelete={props.onDelete} item={e}/>))}
                </tbody>
            </Table>

            <DialogAddMaterial
                projectId={props.projectId}
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
                onSubmit={props.onUpdate}
            />
        </>
    )
}

export default MaterialList;