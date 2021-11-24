import {useState} from "react";
import { Table} from "react-bootstrap";
import {DbLoadCase} from "./DatabaseInterface";
import DialogAddLoadCase from "./DialogAddLoadCase";
import {ListProps} from "./ListProps";
import NewButton from "./ui/NewButton";
import EditButton from "./ui/EditButton";
import CopyButton from "./ui/CopyButton";
import RemoveButton from "./ui/RemoveButton";
import {ListItemProps} from "./ListItemProps";

const LoadCaseListItem = (props: ListItemProps<DbLoadCase>) => {
    return (
        <tr>
            <td>{props.item.name}</td>
            <td>{props.item.description}</td>
            <td>{props.item.actionType}</td>
            <td>{props.item.type}</td>
            <td><EditButton onClick={() => {}} disabled={true}/></td>
            <td><CopyButton onClick={() => {}} disabled={true}/></td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))} disabled={true}/></td>
        </tr>
    )
}

const LoadCaseList = (props: ListProps<DbLoadCase>) => {

    const [showNewDialog, setShowNewDialog] = useState(false);

    return (
        <>
                <NewButton onClick={() => setShowNewDialog(true)} disabled={false} title={"Loadcases"}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th>Name</th>
                    <th>Description</th>
                    <th>Action</th>
                    <th>Type</th>
                    <th></th>
                    <th></th>
                    <th></th>
                </tr>
                </thead>
                <tbody>
                {props.items.map(e => (<LoadCaseListItem onDelete={props.onDelete} item={e}/>))}
                </tbody>
            </Table>

            <DialogAddLoadCase
                projectId={props.projectId}
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
            />
        </>
    )
}

export default LoadCaseList;