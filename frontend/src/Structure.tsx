import React, { useEffect, useState } from "react";
import { Button, Spinner, Table } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import { BackButton } from "./ui/BackButton";
import { DbStructuralPoint } from "./DatabaseInterface";
import axios from 'axios';
import { CopyButton } from "./ui/CopyButton";
import { RemoveButton } from "./ui/RemoveButton";
import { EditButton } from "./ui/EditButton";

type StructuralPointsListItemProps = {
    projectId: string;
    id: number;
    name: string;
    uuid: string;
    coordinate_x: number;
    coordinate_y: number;
    coordinate_z: number;
    onDelete: (id: number) => void;
}

function StructuralPointsListItem(props: StructuralPointsListItemProps) {
    const history = useHistory();

    const onEdit = () => { history.push("/editstructuralpoint/" + props.id); }
    const onCopy = () => {
        console.log("on copy")
    }
    const onDelete = () => { props.onDelete(props.id) }
    return (
        <tr>
            <td><EditButton onClick={onEdit} disabled={false}></EditButton></td>
            <td>{props.name}</td>
            <td>{props.coordinate_x}</td>   
            <td>{props.coordinate_y}</td>
            <td>{props.coordinate_z}</td>
            <td>{props.uuid}</td>
            <td><CopyButton onClick={onCopy} disabled={false} /></td>
            <td><RemoveButton onClick={onDelete} disabled={false} /></td>
        </tr>
    )
}

type StructuralPointsListProps = {
    projectId: string
    data: DbStructuralPoint[]
    onDelete: (id: number) => void;
}

function StructuralPointsList(props: StructuralPointsListProps) {
    return ( 
        <>
        <Table borderless hover size="sm">
            <thead>
                <tr>
                    <th></th>
                    <th>Name</th>
                    <th>X</th>
                    <th>Y</th>
                    <th>Z</th>
                    <th>UUID</th>
                </tr>
            </thead>
            <tbody>
                {props.data.map(e => (
                    <StructuralPointsListItem 
                        projectId={props.projectId}
                        id={e.id} 
                        name={e.name} 
                        uuid={e.uuid} 
                        coordinate_x={e.coordinate_x} 
                        coordinate_y={e.coordinate_y} 
                        coordinate_z={e.coordinate_z} 
                        onDelete={props.onDelete}
                    />))}
            </tbody>
        </Table>
    </>)
}

export type StructureProps = {
    projectId: string
}

export default function Structure(props: StructureProps) {

    const history = useHistory();
    const [structuralPoints, setstructuralPoints] = useState<DbStructuralPoint[]>([])
    const [loading, setLoading] = useState(true)

    const onBack = () => { history.push("/project/" + props.projectId); }
    const onNewPoint = () => { history.push("/structuralpoint/" + props.projectId); }

    const onDeletePoint = (id: number) => {
        axios.delete(process.env.BACKEND + '/structuralpoint/' + id)
        window.location.reload();
    }

    async function getData() {
        await axios.get(process.env.BACKEND + '/structuralpoints/' + props.projectId)
            .then(response => setstructuralPoints(response.data))
            .finally(() => setLoading(false))
    }

    useEffect(() => { getData() }, []);

    if (loading) {
        return (<Spinner animation="border" role="status"></Spinner>);
    }

    return (
        <>
            <BackButton onClick={onBack} disabled={false}/>
            <h3>Structural Points</h3>
            <Button onClick={onNewPoint}>New point...</Button>
            <StructuralPointsList projectId={props.projectId} data={structuralPoints} onDelete={onDeletePoint}/>
            <li>Member</li>
        </>
    );
}

export interface RouterComponentStructureProps extends RouteComponentProps<StructureProps> { }
