import {useState} from "react";
import {Table} from "react-bootstrap";
import {DbCrossSection} from "./DatabaseInterface";
import DialogAddCrossSection from "./DialogAddCrossSection";
import {ListProps} from "./ListProps";
import {ListItemProps} from "./ListItemProps";
import NewButton from "./ui/NewButton";
import EditButton from "./ui/EditButton";
import CopyButton from "./ui/CopyButton";
import RemoveButton from "./ui/RemoveButton";

const CrossSectionListItem = (props: ListItemProps<DbCrossSection>) => {
    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.material.name}</td>
            <td>{props.item.area}</td>
            <td>{props.item.systemLine}</td>
            <td><EditButton onClick={() => {
            }} disabled={true}/></td>
            <td><CopyButton onClick={() => {
            }} disabled={true}/></td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))} disabled={false}/></td>
        </tr>
    )
}

const CrossSectionList = (props: ListProps<DbCrossSection>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false} title={"Cross Sections"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>Material</th>
                    <th>Area [m2]</th>
                    <th>System line</th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<CrossSectionListItem onDelete={props.onDelete} item={e} />))}
                </tbody>
            </Table>

            <DialogAddCrossSection
                projectId={props.projectId}
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
                onSubmit={props.onUpdate}
            />
        </>
    )
}

export default CrossSectionList;
